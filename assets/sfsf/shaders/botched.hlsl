// =============================================================================
//  Breath of the Wild style post-process  --  D3D9 / ps_3_0
// =============================================================================
//
//  Five stages, in the order they run:
//
//    1. World reconstruction   depth -> world position -> true metric distance
//    2. Outlines               curvature + crease detection, distance weighted
//    3. Cel shading            hue-preserving luminance banding
//    4. Aerial perspective     the pale haze that makes BotW read as "big"
//    5. Light shafts           radial blur from a virtual sun, dithered
//
//  The single most important change from a naive port is stage 1. Raw INTZ
//  depth is z/w, which is wildly non-linear: it burns most of its precision in
//  the first few metres. Any threshold picked against it - outlines, fog, sky -
//  is only correct at one distance and wrong everywhere else. Reconstructing
//  world position with the inverse view-projection gives real metres, so every
//  threshold below is a distance in world units and behaves the same whether
//  you are looking at your boots or at a mountain.
//
//  Registers supplied by the framework:
//    s0     = colour capture
//    s1     = INTZ depth (value in .r)
//    c0     = (width, height, seconds, aspect)
//    c1-c4  = inverse view-projection
//    c5     = camera world position, .w = 1 when valid
//    c6     = light 0 world position, .w = 1 when valid (unused, engine gives 0)
//    c7-c10 = forward view-projection
//    c11    = sun direction, .w = 1 when it looks like a unit vector
// =============================================================================

sampler2D uTexture : register(s0);
sampler2D uDepth   : register(s1);

float4 uParams : register(c0);
float4 uCamera : register(c5);
float4 uSunDir : register(c11);

// Inverse view-projection: clip -> world.
float4 uIVP0 : register(c1);
float4 uIVP1 : register(c2);
float4 uIVP2 : register(c3);
float4 uIVP3 : register(c4);

// Forward view-projection: world -> clip.
float4 uVP0 : register(c7);
float4 uVP1 : register(c8);
float4 uVP2 : register(c9);
float4 uVP3 : register(c10);

#define IVP_ROW_MAJOR   1

#define USE_WORLD_RECON 1

#define FALLBACK_NEAR    1.0
#define FALLBACK_FAR  8000.0
#define FALLBACK_FOV    65.0   // vertical FOV in degrees, for the synthetic ray


// --- Depth source ------------------------------------------------------------
// Run DEBUG_MODE 8 / 9 / 10 before touching anything else here. In order they
// answer: is there a picture in the depth buffer at all, is there a picture
// hiding in a range too narrow to see, and is the buffer simply cleared?
//
// If only UI elements have depth and the world is flat, no setting below will
// help - the surface bound to s1 is not the one the world was drawn into, and
// the fix is on the framework side. See the SKY/EDGE/FOG fallbacks further down
// for how to get the look anyway.
#define DEPTH_CHANNEL   0      // 0 = .r, 1 = .g, 2 = .b, 3 = .a
#define DEPTH_INVERT    0      // 1 if the buffer is reversed-Z (near = 1, far = 0)

// Stretch a narrow band of the buffer across the full 0..1 range. If DEBUG 9
// shows the whole world crammed between, say, 0.98 and 1.0, set these to that
// band and every threshold in the shader becomes usable again.
#define DEPTH_REMAP_LO  0.0
#define DEPTH_REMAP_HI  1.0


// --- Depth-free fallbacks ----------------------------------------------------
// Set all three to 1 when the depth buffer is unusable. The look survives
// surprisingly well: outlines come off colour discontinuities instead of
// silhouettes, haze comes off screen height instead of distance, and the sky
// is found by brightness. It is cruder - texture detail picks up lines that a
// depth silhouette would have ignored - but it runs anywhere.
#define EDGE_SOURCE     0      // 0 = depth silhouettes, 1 = colour, 2 = both
#define FOG_SOURCE      0      // 0 = true distance, 1 = screen height
#define SKY_SOURCE      0      // 0 = depth threshold, 1 = brightness + height

#define COLOUR_EDGE_TOL 0.835   // EDGE_SOURCE 1/2: lower = more lines
#define SKY_LUMA_LO     0.05   // SKY_SOURCE 1: brightness that starts reading as sky
#define SKY_LUMA_HI     0.78
#define FOG_HORIZON     0.42   // FOG_SOURCE 1: screen height of the horizon, 0 = top
#define FOG_HORIZON_FADE 0.28  // how far below it the haze takes to clear


// --- Sun placement -----------------------------------------------------------
#define SUN_SOURCE     1

// c11 holds the direction light TRAVELS (z negative - pointing down), so it must
// be negated to get the direction TO the sun. Without this the virtual sun ends
// up underground: no rays looking horizontally, and the screen washes out as the
// camera tilts toward it.
#define SUN_INVERT     1

// Used when SUN_SOURCE is 0, or when c11 is not valid.
// World axes: x and y are horizontal (tile coords), z is up.
#define SUN_AZIMUTH    135.0   // degrees around the vertical axis
#define SUN_ELEVATION   38.0   // degrees above the horizon
#define SUN_DISTANCE  4000.0   // how far to place the virtual sun


// --- Sky detection -----------------------------------------------------------
// These are thresholds against RAW z/w, which is savagely non-linear: in a
// typical D3D9 projection everything past a few dozen metres already sits above
// 0.99, and the skybox pins at 1.0. So the only safe place to put a sky cutoff
// is right up against the ceiling.
//
// The inherited 0.65 / 0.70 looks reasonable and is catastrophically wrong: it
// classifies all terrain as sky, and since sky suppresses cel shading, outlines
// and fog alike, it switches off the entire effect on everything except UI.
//
// Verify with DEBUG_MODE 1 - only the skybox should be white, and the horizon
// line should land exactly where terrain meets sky.
#define SKY_EDGE_LO  0.9990
#define SKY_EDGE_HI  0.9998



// --- Cel shading -------------------------------------------------------------
// Quantise LUMINANCE and rescale the colour, never the channels independently.
// Per-channel banding lands R, G and B on different steps, which drags hues
// sideways - skin goes green, foliage goes yellow. This keeps hue and chroma
// intact and only staircases brightness, which is what cel shading actually is.
#define CELL_LEVELS      7.0   // number of brightness bands
#define BAND_SOFTNESS    0.16  // 0 = razor steps, 0.5 = no banding at all
#define CELL_STRENGTH    0.80  // blend back toward the original, 1 = full banding
#define SHADOW_FLOOR     0.14  // lifted blacks - BotW shadows are never crushed

// --- Outlines ----------------------------------------------------------------
// Two detectors, taken as a max:
//   curvature - second derivative of distance. A flat receding ground plane has
//               a huge first derivative but near-zero second, so this finds real
//               silhouettes without painting lines all over the terrain.
//   crease    - angle between normals fitted on either side of the pixel. Finds
//               wall corners and rock facets, where depth is continuous.
#define OUTLINE_WIDTH      3.0   // tap offset in pixels
#define OUTLINE_CURVE_TOL  0.020 // relative curvature that counts as an edge
#define OUTLINE_CREASE_TOL 0.35  // 1 - dot(n0, n1) that counts as a crease
#define OUTLINE_SOFTNESS   0.5   // edge AA, 0 = hard step
#define OUTLINE_NEAR       0.55  // line strength on nearby geometry
#define OUTLINE_FAR        0.80  // line strength at range - BotW leans on these
#define OUTLINE_FAR_DIST 900.0   // distance in world units where FAR is reached
#define OUTLINE_DARKEN     0.12  // multiplier on the surface's own colour
#define OUTLINE_HAZE_MIX   0.45  // pull distant lines toward the haze colour


// --- Aerial perspective ------------------------------------------------------
// The real signature of the look. Distant geometry washes to a pale blue, and
// the haze warms up where you are looking into the sun.
#define FOG_START       120.0
#define FOG_DENSITY    0.00055
#define FOG_CURVE        1.25  // >1 keeps middle distance clear, then rolls off
#define FOG_MAX          0.88  // never fully erase geometry
#define FOG_COLOR     float3(0.25, 0.45, 0.85)
#define FOG_SUN_COLOR float3(0.98, 0.88, 1)
#define FOG_SUN_POWER    6.0

#define SUN_GLOW          0.05  // broad halo around the sun in open sky
#define SUN_GLOW_POWER   2.0


// --- Colour grade ------------------------------------------------------------
#define VIBRANCY_BOOST    1.3
#define SHADOW_TINT    float3(0.85, 0.9, 1.0)
#define HIGHLIGHT_TINT float3(0.7, 0.84, 1)


// --- Shafts ------------------------------------------------------------------
#define NUM_SAMPLES     48
#define DENSITY       1.20
// DECAY is per-sample and compounds over NUM_SAMPLES, so it is far more
// sensitive than it looks: 0.98 still retains ~38% at sample 48, while 0.90 is
// down to 0.6% and 0.3 is gone by sample 6. Anything much below ~0.95 turns
// long shafts into short stubs around the sun. Reach lives here, not in WEIGHT.
#define DECAY         0.04
#define WEIGHT        0.50
#define EXPOSURE      0.20
// This is what makes shafts directional rather than a smear. It selects which
// pixels are bright enough to count as a light source. Drop it toward 0 and
// every mid-tone in the frame seeds a shaft, so the pass degenerates into a
// plain radial blur of the whole image - which both washes the screen out and
// destroys the beams, because beams need contrast against unlit air.
#define RAY_THRESHOLD 0.50
#define RAY_TINT      float3(0.90, 0.94, 1.00)
#define RAY_DITHER    1        // jitter the march start to kill ring banding
// Shafts fade out as the sun leaves the frame, rather than snapping off.
#define OFFSCREEN_FADE 0.01    // screen widths beyond the edge before fully gone


// --- Volumetric shafts -------------------------------------------------------
// The radial blur above can only smear light that is already on screen, so it
// dies the moment the sun leaves the frame. This marches the view ray instead
// and asks, at each step, whether that point in the air can see the sun. It
// works with the sun anywhere - off screen, behind you - and because the answer
// varies along the ray it produces actual shadow beams rather than a glow.
//
// The catch: only geometry that is ON SCREEN can occlude. A mountain just out
// of frame casts nothing. Sample points that project outside the view are
// treated as lit, since the alternative is a hard shadow edge nailed to the
// border of the screen.
//
// 0 = radial only (old behaviour), 1 = volumetric only, 2 = both
// 0 = radial only (old behaviour), 1 = volumetric only, 2 = both
#define SHAFT_MODE      1

#define VOL_STEPS         24   // samples along the view ray
#define VOL_SHADOW_STEPS   7   // samples toward the sun, per view sample
#define VOL_MAX_DIST   6000.0   // how far down the ray to march, world units
#define VOL_SHADOW_LEN  130.0   // how far toward the sun to look for occluders

// VOL_MAX_DIST and VOL_SHADOW_LEN are in whatever units DEBUG_MODE 4 reports.
// If the shafts look microscopic or swallow the screen, these two are why.

#define VOL_DENSITY      0.0016
#define VOL_EXTINCTION   0.9    // how fast the air self-shadows with distance
#define VOL_HEIGHT_BASE  0.0    // world z where the haze is thickest
#define VOL_HEIGHT_FALL  0.0035 // thinning per unit of height, 0 = uniform
#define VOL_INTENSITY    1.5
#define VOL_TINT      float3(0.90, 0.94, 1.00)

#define VOL_HG_G         0.72   // forward scattering, 0 = even, 0.9 = tight beam
#define VOL_AMBIENT      0.30   // floor under the phase term - this is what keeps
                                // shafts alive when you are not facing the sun

#define VOL_SHADOW_BIAS      1.0   // ignore occluders this close to the sample
#define VOL_SHADOW_THICK    80.0   // ...and this far behind it. Stops unrelated
                                   // foreground objects casting phantom beams.
#define VOL_SHADOW_STRENGTH  1.0   // 0 = no shadowing, just uniform haze



// --- Debug -------------------------------------------------------------------
//  0 = normal            1 = sky mask          2 = sun position
//  3 = rays only         4 = linear distance   5 = reconstructed normals
//  6 = outline mask      7 = fog factor
//  8 = raw depth         9 = depth contours   10 = depth classification
// 11 = volumetric only  12 = sun visibility
//
// 8, 9 and 10 are the depth diagnostics - start there if outlines are missing.
// 12 is the one to check if the shafts look wrong: it shows, per pixel, whether
// that surface can see the sun.
#define DEBUG_MODE 0
// =============================================================================


float luminance(float3 c)
{
    return dot(c, float3(0.2126, 0.7152, 0.0722));
}

float hash12(float2 p)
{
    return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

/**
 * Depth exactly as the buffer holds it, with only channel and polarity fixed.
 *
 * Unprojection MUST use this one. The inverse projection expects true clip
 * space z; feeding it a remapped value silently bends the reconstructed world
 * and every distance derived from it.
 */
float raw_depth(float2 uv)
{
    float4 t = tex2D(uDepth, uv);

#if   DEPTH_CHANNEL == 1
    float d = t.g;
#elif DEPTH_CHANNEL == 2
    float d = t.b;
#elif DEPTH_CHANNEL == 3
    float d = t.a;
#else
    float d = t.r;
#endif

#if DEPTH_INVERT
    d = 1.0 - d;
#endif

    return d;
}

/**
 * Depth stretched into a comfortable 0..1 for thresholds and debug views only.
 */
float sample_depth(float2 uv)
{
    return saturate((raw_depth(uv) - DEPTH_REMAP_LO)
                    / max(DEPTH_REMAP_HI - DEPTH_REMAP_LO, 0.000001));
}

float sky_mask(float2 uv)
{
#if SKY_SOURCE == 1
    // No depth to lean on: sky is the bright stuff in the upper part of the
    // frame. Crude, but it fails in a forgiving direction - a bright wall high
    // in frame seeding a few extra shafts is not a visible bug.
    float bright = smoothstep(SKY_LUMA_LO, SKY_LUMA_HI,
                              luminance(tex2D(uTexture, uv).rgb));
    float high   = smoothstep(FOG_HORIZON + 0.15, FOG_HORIZON - 0.05, uv.y);
    return bright * high;
#else
    return smoothstep(SKY_EDGE_LO, SKY_EDGE_HI, raw_depth(uv));
#endif
}

// Fallback linearisation for when the inverse matrix is unavailable. Standard
// D3D projection: z_ndc = (f / (f - n)) * (1 - n / z_view), inverted.
float fallback_distance(float depth)
{
    float denom = FALLBACK_FAR - depth * (FALLBACK_FAR - FALLBACK_NEAR);
    return (FALLBACK_NEAR * FALLBACK_FAR) / max(denom, 0.0001);
}

/**
 * Unproject a pixel back into the world.
 * Returns world position in .xyz and metric distance from the eye in .w.
 */
float4 world_point(float2 uv, float depth)
{
#if USE_WORLD_RECON
    if (uCamera.w > 0.5)
    {
        float2 ndc  = float2(uv.x * 2.0 - 1.0, 1.0 - uv.y * 2.0);
        float4 clip = float4(ndc, depth, 1.0);

    #if IVP_ROW_MAJOR
        float4 w = float4(dot(uIVP0, clip), dot(uIVP1, clip),
                          dot(uIVP2, clip), dot(uIVP3, clip));
    #else
        float4 w = uIVP0 * clip.x + uIVP1 * clip.y
                 + uIVP2 * clip.z + uIVP3 * clip.w;
    #endif

        if (abs(w.w) > 0.000001)
        {
            float3 pos = w.xyz / w.w;
            return float4(pos, length(pos - uCamera.xyz));
        }
    }
#endif

    // No usable matrix. Build a position on a synthetic view ray instead of a
    // bare distance: it is not the real world, but parallax between neighbouring
    // pixels is preserved, and that is all the curvature and normal fits need.
    // Returning a straight (0, 0, d) here makes every neighbour collinear, the
    // cross products collapse to zero, and normals come out as NaN - which is
    // what an empty DEBUG_MODE 5 looks like.
    float  d   = fallback_distance(depth);
    float  t   = tan(radians(FALLBACK_FOV * 0.5));
    float3 ray = float3((uv.x * 2.0 - 1.0) * uParams.w * t,
                        (1.0 - uv.y * 2.0) * t,
                        1.0);
    float3 pos = ray * d;
    return float4(pos, length(pos));
}

float4 world_point_at(float2 uv)
{
    return world_point(uv, raw_depth(uv));
}


float3 sun_direction()
{
#if SUN_SOURCE == 1
    if (uSunDir.w > 0.5)
    {
    #if SUN_INVERT
        return normalize(-uSunDir.xyz);
    #else
        return normalize(uSunDir.xyz);
    #endif
    }
#endif

    float az = radians(SUN_AZIMUTH);
    float el = radians(SUN_ELEVATION);
    return normalize(float3(cos(az) * cos(el), sin(az) * cos(el), sin(el)));
}

/**
 * Screen position of the virtual sun, plus a 0..1 factor that fades the effect
 * as it goes off screen or behind the camera.
 *
 * A directional sun has no position - it is infinitely far away - so the trick
 * is to place a virtual sun a long way along the sun direction FROM THE CAMERA
 * and project that. Because the anchor moves with the camera, the shafts stay
 * correct as you pan, rotate and zoom, which a fixed screen position cannot do.
 */
float2 sun_screen_pos(out float visibility)
{
    visibility = 0.0;

    if (uCamera.w < 0.5)
    {
        return float2(0.5, 0.05);   // no camera - nothing sensible to aim at
    }

    float3 direction = sun_direction();

    /* A sun below the horizon means the direction convention is wrong, and the
     * symptom is the screen washing out when the camera tilts down. Bail rather
     * than render that. */
    if (direction.z <= 0.0)
    {
        return float2(0.5, 0.05);
    }

    float4 world = float4(uCamera.xyz + direction * SUN_DISTANCE, 1.0);
    float4 clip  = float4(dot(uVP0, world), dot(uVP1, world),
                          dot(uVP2, world), dot(uVP3, world));

    if (clip.w <= 0.0001)
    {
        return float2(0.5, 0.05);   // behind the eye, visibility stays 0
    }

    float2 ndc = clip.xy / clip.w;
    float2 uv  = float2(ndc.x * 0.5 + 0.5, 0.5 - ndc.y * 0.5);

    // Distance outside the [0,1] box, in screen widths.
    float2 outside = max(max(-uv, uv - 1.0), 0.0);
    visibility = 1.0 - saturate(length(outside) / OFFSCREEN_FADE);

    return uv;
}


// -----------------------------------------------------------------------------
//  Outlines
// -----------------------------------------------------------------------------
/**
 * Outlines from the colour buffer alone, for when depth is not available.
 *
 * Same curvature trick as the depth path, and for the same reason: a smooth
 * lighting gradient across a lit surface cancels out, a hard boundary does not.
 * That keeps it off soft shading while still catching object borders.
 */
float colour_edge(float2 uv)
{
    float2 texel = float2(OUTLINE_WIDTH / uParams.x, OUTLINE_WIDTH / uParams.y);

    float3 c = tex2D(uTexture, uv).rgb;
    float3 r = tex2D(uTexture, uv + float2( texel.x, 0.0)).rgb;
    float3 l = tex2D(uTexture, uv + float2(-texel.x, 0.0)).rgb;
    float3 d = tex2D(uTexture, uv + float2(0.0,  texel.y)).rgb;
    float3 u = tex2D(uTexture, uv + float2(0.0, -texel.y)).rgb;

    float3 curve = abs(l + r - 2.0 * c) + abs(u + d - 2.0 * c);
    float  e     = max(max(curve.r, curve.g), curve.b);

    // Normalise against local brightness, or dark corners never outline while
    // lit surfaces outline constantly.
    e /= max(luminance(c), 0.25);

    return smoothstep(COLOUR_EDGE_TOL, COLOUR_EDGE_TOL * 2.0, e);
}

/**
 * Edge strength at uv. Also hands back the reconstructed normal, because the
 * four neighbour taps are expensive and the debug views want them anyway.
 */
float detect_edge(float2 uv, float4 centre, out float3 normal)
{
    float2 texel = float2(OUTLINE_WIDTH / uParams.x, OUTLINE_WIDTH / uParams.y);

    float4 pR = world_point_at(uv + float2( texel.x, 0.0));
    float4 pL = world_point_at(uv + float2(-texel.x, 0.0));
    float4 pD = world_point_at(uv + float2(0.0,  texel.y));
    float4 pU = world_point_at(uv + float2(0.0, -texel.y));

    // Second derivative of distance. Flat surfaces cancel out no matter how
    // steeply they are angled away; only genuine discontinuities survive.
    float curve = abs(pL.w + pR.w - 2.0 * centre.w)
                + abs(pU.w + pD.w - 2.0 * centre.w);
    float curve_edge = curve / (centre.w * OUTLINE_CURVE_TOL);

    // Fit a normal on each side of the pixel and compare. Equal on a flat
    // surface, sharply different across a crease.
    float3 nA = normalize(cross(pR.xyz - centre.xyz, pD.xyz - centre.xyz));
    float3 nB = normalize(cross(centre.xyz - pL.xyz, centre.xyz - pU.xyz));
    normal = normalize(nA + nB);

    float crease_edge = (1.0 - saturate(dot(nA, nB))) / OUTLINE_CREASE_TOL;

    float edge = max(curve_edge, crease_edge);
    return smoothstep(1.0 - OUTLINE_SOFTNESS, 1.0 + OUTLINE_SOFTNESS, edge);
}


// -----------------------------------------------------------------------------
//  Cel shading and grading
// -----------------------------------------------------------------------------
float3 cel_shade(float3 col)
{
    float l = luminance(col);

    // Lift before banding so the darkest step lands on SHADOW_FLOOR rather than
    // on black - a hard zero band reads as a hole, not a shadow.
    float lifted = lerp(SHADOW_FLOOR, 1.0, l);

    float scaled = lifted * CELL_LEVELS;
    float step_i = floor(scaled);
    float f      = frac(scaled);

    // Soft staircase: a smoothstep across the middle of each step gives the
    // band edge a pixel or two of AA instead of a jagged contour line.
    float banded = (step_i + smoothstep(0.5 - BAND_SOFTNESS,
                                        0.5 + BAND_SOFTNESS, f)) / CELL_LEVELS;
    banded = max(banded, SHADOW_FLOOR);

    // Rescale rather than replace - hue and saturation ride along untouched.
    float3 shaded = col * (banded / max(l, 0.0001));
    return lerp(col, shaded, CELL_STRENGTH);
}

float3 fantasy_grade(float3 col)
{
    col = max(col, SHADOW_FLOOR);

    float luma = luminance(col);
    col *= lerp(SHADOW_TINT, HIGHLIGHT_TINT, luma);

    col = saturate(col);
    col = col * col * (3.0 - 2.0 * col);        // gentle S-curve for contrast

    return saturate(lerp(luma.xxx, col, VIBRANCY_BOOST));
}


// -----------------------------------------------------------------------------
//  Aerial perspective
// -----------------------------------------------------------------------------
float fog_amount(float dist, float2 uv)
{
#if FOG_SOURCE == 1
    // Stopgap: haze by screen height. Wrong in principle - a near wall in the
    // upper half of the frame gets hazed as if it were a mountain - but for
    // landscape views it reads convincingly, and it needs no depth at all.
    return smoothstep(FOG_HORIZON + FOG_HORIZON_FADE, FOG_HORIZON, uv.y) * FOG_MAX;
#else
    float d = max(dist - FOG_START, 0.0) * FOG_DENSITY;
    return saturate((1.0 - exp(-pow(d, FOG_CURVE))) * FOG_MAX);
#endif
}

float3 haze_color(float3 view_dir, float3 sun_dir)
{
    // Forward scattering: haze between you and the sun lights up.
    float phase = pow(saturate(dot(view_dir, sun_dir)), FOG_SUN_POWER);
    return lerp(FOG_COLOR, FOG_SUN_COLOR, phase);
}


// -----------------------------------------------------------------------------
//  Volumetric shafts
// -----------------------------------------------------------------------------
/**
 * Depth read for use inside the marching loops.
 *
 * tex2D derives its mip level from screen-space derivatives, which are
 * undefined inside dynamic flow control - fxc will either refuse the shader or
 * quietly produce garbage at the edges. tex2Dlod states the level outright and
 * sidesteps the whole problem.
 */
float depth_lod(float2 uv)
{
    float4 t = tex2Dlod(uDepth, float4(uv, 0.0, 0.0));

#if   DEPTH_CHANNEL == 1
    float d = t.g;
#elif DEPTH_CHANNEL == 2
    float d = t.b;
#elif DEPTH_CHANNEL == 3
    float d = t.a;
#else
    float d = t.r;
#endif

#if DEPTH_INVERT
    d = 1.0 - d;
#endif

    return d;
}

/**
 * Henyey-Greenstein phase function: how much light scatters toward the eye at a
 * given angle from the sun. Sharply forward biased, which is why haze blazes
 * when you look into the sun and merely glows when you do not.
 */
float hg_phase(float cos_theta, float g)
{
    float g2 = g * g;
    float denom = 1.0 + g2 - 2.0 * g * cos_theta;
    return (1.0 - g2) / pow(max(denom, 0.0001), 1.5);
}

float vol_density(float3 world_pos)
{
    // Haze pools low and thins with altitude. World z is up in this engine.
    float h = max(world_pos.z - VOL_HEIGHT_BASE, 0.0);
    return VOL_DENSITY * exp(-h * VOL_HEIGHT_FALL);
}

/**
 * Can this point in the air see the sun?
 *
 * Steps toward the sun and asks the depth buffer whether anything is standing
 * in the way. Entirely branchless: every test folds into a multiply, because
 * divergent branches around texture reads are exactly what ps_3_0 dislikes.
 */
float sun_visibility(float3 p, float3 sun_dir)
{
    if (uCamera.w < 0.5)
    {
        return 1.0;
    }

    float3 step_v = sun_dir * (VOL_SHADOW_LEN / VOL_SHADOW_STEPS);
    float3 s      = p;
    float  shadow = 0.0;

    [loop]
    for (int j = 0; j < VOL_SHADOW_STEPS; j++)
    {
        s += step_v;

        float4 w    = float4(s, 1.0);
        float4 clip = float4(dot(uVP0, w), dot(uVP1, w),
                             dot(uVP2, w), dot(uVP3, w));

        float  valid = step(0.0001, clip.w);
        float2 ndc   = clip.xy / max(clip.w, 0.0001);
        float2 suv   = float2(ndc.x * 0.5 + 0.5, 0.5 - ndc.y * 0.5);

        float2 in2    = step(0.0, suv) * step(suv, 1.0);
        float  inside = in2.x * in2.y * valid;

        float scene_d  = world_point(suv, depth_lod(suv)).w;
        float sample_d = length(s - uCamera.xyz);
        float diff     = sample_d - scene_d;

        // Occluded if the sample sits behind visible geometry - but only just
        // behind it. Without the far limit, any foreground object throws a beam
        // across everything behind it, whether it is near the light path or not.
        float hit = step(VOL_SHADOW_BIAS, diff) * step(diff, VOL_SHADOW_THICK);

        shadow = max(shadow, hit * inside);
    }

    return 1.0 - shadow * VOL_SHADOW_STRENGTH;
}

/**
 * Inscattered sunlight along the view ray. Returns light to ADD to the scene.
 */
float3 volumetric_light(float dist, float3 view_dir, float3 sun_dir, float jitter)
{
    if (uCamera.w < 0.5)
    {
        return 0.0;
    }

    // Stop at the first surface, or at the range limit for sky pixels - which
    // are effectively at infinity and would otherwise march forever.
    float march = min(dist, VOL_MAX_DIST);
    float step_len = march / VOL_STEPS;

    float3 step_v = view_dir * step_len;

    // Offsetting each pixel's first sample by a fraction of a step trades hard
    // banding across the beams for fine noise, which reads as haze.
    float3 pos = uCamera.xyz + view_dir * (step_len * jitter);

    float phase = hg_phase(dot(view_dir, sun_dir), VOL_HG_G) + VOL_AMBIENT;

    float scatter       = 0.0;
    float transmittance = 1.0;

    [loop]
    for (int i = 0; i < VOL_STEPS; i++)
    {
        pos += step_v;

        float d   = vol_density(pos) * step_len;
        float lit = sun_visibility(pos, sun_dir);

        scatter       += lit * d * transmittance;
        transmittance *= exp(-d * VOL_EXTINCTION);
    }

    return VOL_TINT * scatter * phase * VOL_INTENSITY;
}


// -----------------------------------------------------------------------------
float4 main(float2 uv : TEXCOORD0) : COLOR0
{
    float3 raw_scene = tex2D(uTexture, uv).rgb;
    float  depth     = raw_depth(uv);
    float  depth_n   = sample_depth(uv);

    float4 centre  = world_point(uv, depth);
    float  dist    = centre.w;
    float  sky     = sky_mask(uv);

    float3 sun_dir  = sun_direction();
    float3 view_dir = (uCamera.w > 0.5)
                    ? normalize(centre.xyz - uCamera.xyz)
                    : float3(0.0, 0.0, 1.0);

    // --- grade, then band -----------------------------------------------------
    // Sky keeps its smooth gradient. Banding a sunset into five steps is the
    // fastest way to make the whole thing look cheap rather than stylised.
    float3 graded = fantasy_grade(raw_scene);
    float3 scene  = lerp(cel_shade(graded), graded, sky);

    // --- outlines -------------------------------------------------------------
    float3 normal = float3(0.0, 0.0, 1.0);

#if EDGE_SOURCE == 1
    float edge = colour_edge(uv);
#elif EDGE_SOURCE == 2
    float edge = max(detect_edge(uv, centre, normal), colour_edge(uv));
#else
    float edge = detect_edge(uv, centre, normal);
#endif

    // Distance weighting only means something if the distance is real.
#if EDGE_SOURCE == 1
    float weight = OUTLINE_FAR;
#else
    float weight = lerp(OUTLINE_NEAR, OUTLINE_FAR,
                        saturate(dist / OUTLINE_FAR_DIST));
#endif

    float  line_a = edge * weight * (1.0 - sky);
    float  fog_t  = fog_amount(dist, uv);

    float3 haze   = haze_color(view_dir, sun_dir);
    float3 ink    = lerp(scene * OUTLINE_DARKEN,
                         haze * OUTLINE_DARKEN,
                         OUTLINE_HAZE_MIX * fog_t);
    scene = lerp(scene, ink, line_a);

    // --- aerial perspective ---------------------------------------------------
    float fog = fog_t * (1.0 - sky);
    scene = lerp(scene, haze, fog);

    // Broad halo in open sky, so the shafts have something to emanate from.
    float glow = pow(saturate(dot(view_dir, sun_dir)), SUN_GLOW_POWER);
    scene += FOG_SUN_COLOR * glow * SUN_GLOW * sky;

    float  visibility;
    float2 sun_uv = sun_screen_pos(visibility);

#if DEBUG_MODE == 1
    return float4(sky.xxx, 1.0);
#elif DEBUG_MODE == 2
    float2 d = abs(uv - sun_uv);
    float  cross_mark = 1.0 - saturate(min(d.x, d.y) * 200.0);
    float3 marker = (uSunDir.w > 0.5 && SUN_SOURCE == 1)
                    ? float3(0.0, 1.0, 0.0) : float3(1.0, 0.0, 0.0);
    marker *= 0.25 + 0.75 * visibility;
    return float4(lerp(scene, marker, cross_mark), 1.0);
#elif DEBUG_MODE == 4
    // Banded distance ramp. Should march smoothly out to the horizon; if it
    // saturates a metre in front of the camera the matrix convention is wrong.
    float ramp = frac(dist / 100.0);
    return float4(dist / OUTLINE_FAR_DIST, ramp, 1.0 - sky, 1.0);
#elif DEBUG_MODE == 5
    return float4(normal * 0.5 + 0.5, 1.0);
#elif DEBUG_MODE == 6
    return float4(line_a.xxx, 1.0);
#elif DEBUG_MODE == 7
    return float4(fog.xxx, 1.0);

#elif DEBUG_MODE == 8
    // Depth after DEPTH_REMAP, straight to greyscale.
    return float4(depth_n.xxx, 1.0);

#elif DEBUG_MODE == 9
    // The same buffer at three magnifications, because a world squeezed into
    // the top 1% of the range looks identical to an empty one in DEBUG 8.
    //   red   = remapped
    //   green = frac(d * 32)    - coarse contours
    //   blue  = frac(d * 1024)  - fine contours
    // Only two or three coarse bands across the whole terrain means the world
    // lives in a narrow slice of the range - set DEPTH_REMAP_LO/HI to that
    // slice and re-check until the bands spread evenly across the view.
    return float4(depth_n, frac(depth_n * 32.0), frac(depth_n * 1024.0), 1.0);

#elif DEBUG_MODE == 10
    // Classification, against RAW depth so the remap cannot flatter it.
    // Red = pinned near, green = live, blue = pinned far.
    float3 klass = float3(step(depth, 0.0005),
                          step(0.0005, depth) * step(depth, 0.9995),
                          step(0.9995, depth));
    return float4(klass, 1.0);
#endif

    // A single dither value, shared by both shaft techniques.
#if RAY_DITHER
    float jitter = hash12(uv * uParams.xy);
#else
    float jitter = 0.0;
#endif

    // --- volumetric shafts ----------------------------------------------------
    // Note there is no early-out on sun visibility here. That is the entire
    // point: this marches the view ray, so it keeps working with the sun off
    // screen or behind the camera.
    float3 volumetric = 0.0;
#if SHAFT_MODE >= 1
    volumetric = volumetric_light(dist, view_dir, sun_dir, jitter);
#endif

#if DEBUG_MODE == 11
    return float4(volumetric, 1.0);
#elif DEBUG_MODE == 12
    return float4(sun_visibility(centre.xyz, sun_dir).xxx, 1.0);
#endif

    scene += volumetric;

    // --- radial shafts --------------------------------------------------------
    float3 shafts = 0.0;

#if SHAFT_MODE == 0 || SHAFT_MODE == 2
    if (visibility > 0.001)
    {
        // Step toward the sun, aspect-corrected so shafts stay radial.
        float2 delta = uv - sun_uv;
        delta.x *= uParams.w;
        delta *= DENSITY / NUM_SAMPLES;
        delta.x /= uParams.w;

        float2 sample_uv = uv - delta * jitter;
        float  decay     = 1.0;

        for (int i = 0; i < NUM_SAMPLES; i++)
        {
            sample_uv -= delta;

            float2 edge2  = smoothstep(0.0, 0.15, sample_uv)
                          * smoothstep(1.0, 0.85, sample_uv);
            float  inside = edge2.x * edge2.y;

            float3 tap = tex2D(uTexture, sample_uv).rgb;

            // Only bright sky seeds a shaft. Without this the blur smears every
            // lit rooftop into the sky and the rays lose their direction.
            float bright = saturate((luminance(tap) - RAY_THRESHOLD)
                                    / (1.0 - RAY_THRESHOLD));

            shafts += tap * bright * sky_mask(sample_uv) * inside * decay * WEIGHT;
            decay  *= DECAY;
        }

        shafts *= visibility * RAY_TINT;
    }
#endif

#if DEBUG_MODE == 3
    float ray_val = luminance(shafts) * EXPOSURE;
    float pulse = 0.5 + 0.5 * sin(uParams.z * 3.0);
    ray_val = saturate((ray_val * pulse - 0.05) / 0.45);
    return float4(float3(0.0, 1.0, 0.0) * (0.2 + 0.8 * ray_val), 1.0);
#endif

    return float4(saturate(scene + shafts * EXPOSURE), 1.0);
}
