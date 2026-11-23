# rapid fire — example of the RANGED_SHOT on-hit phase

Every ranged shot of a buffed figure spawns extra clones of the donor
projectile: the clones launch as a batch across the shot-completion
animation window (~700 ms = 7 engine ticks, evenly spread), each flies the
donor flight length from its own launch, so they land on the same victim
with the same launch spread and deal the snapshot damage of the original
shot. Each clone also gets its own full vanilla weapon-enchant roll (all 3
slots, independent per arrow); winning procs are scheduled with a
future-start addSpell — the spell instance sits on the victim from the shot
on, and its effect lands at the clone's arrival tick. The volley count and
the buff duration are data-driven.

Framework mechanisms used: the `RANGED_SHOT` phase of the on-hit registry
(dispatched once per ranged shot), `cloneEffect`, and the
`rollWeaponEnchantEffects` helper exposed through `SpellFunctions`.

## Data requirements (gamedata, SFCFF)

Ids: **SpellLine 251**, **Spell 3555**, **SpellJob 177** (job id lives in the
mod code only — no gamedata table for it). Spell params:

- `params[0]` = buff duration in **ms** (shipped value 10000 = 10 s; owner-tunable)
- `params[1]` = extra arrows per shot (shipped value 3; tunable in data
  without recompilation)

**WARNING (verified on the live working gamedata 2026-11-10):** spell id
**3555 is NOT free** — spells 3555-3573 are the vanilla BlackMagic Death
specialisation block (3555 = Lesser Pain lvl 2, `SpellLine 246`). The task
inventory claimed 3555 free; the developer's unpack of the live
`data\gamedata.cff` disproves it. First free spell id is **3574** (ids 1-3573
are taken; count 3481 records). Do NOT overwrite 3555 — pick the free id
confirmed by the owner and use it in the SFCFF commands below (the mod code
reads the spell through `SpellLine 251`, so the spell id itself is not
hardcoded anywhere in code).

Working-copy workflow (all commands decimal; `<root>` = game install root;
keep backups; never edit the live file directly — a prepared working copy
with SpellLine 251 already added lives in the task temp):

1. Backup `<root>\data\gamedata.cff`.
2. `SFCFF.exe u <temp>\gamedata.cff t2054` — SpellLines.
3. Add inside `<SpellLines>` (already applied in the task temp copy,
   verified round-trip):

   ```xml
   <!--SpellLine: UNKNOWN-->
   <!--StringID: Rapid Fire (sf-rapid-fire, SFSF example; UI metadata only)-->
   <SpellLine ID="251" StringID="0" Flags="1" MagicBase="0" MinLevel="1" MaxLevel="20" Availability="0" UIExtTTId="0" MeshName="ui_spell_melee_berserk"/>
   ```

4. `SFCFF.exe u <temp>\gamedata.cff t2002` — Spells. Add inside `<Spells>`
   (mirror of the Lesser Pain spell shape; `<SPELL_ID>` = owner-confirmed
   free id, e.g. 3574):

   ```xml
   <!--SpellLine: UNKNOWN-->
   <Spell ID="<SPELL_ID>" SpellLine="251" ManaUsage="0" CastTime="10" RecastTime="6000" RangeMin="0" RangeMax="15" Faction="2" TargetType="1" EffectPower="100" EffectRange="0">
       <SpellData>
           <Data iValue="10000"/>
           <Data iValue="3"/>
           <Data iValue="0"/>
           <Data iValue="0"/>
           <Data iValue="0"/>
           <Data iValue="0"/>
           <Data iValue="0"/>
           <Data iValue="0"/>
           <Data iValue="0"/>
           <Data iValue="0"/>
       </SpellData>
   </Spell>
   ```

   `SpellData` maps to spell params: `params[0]` = 10000 (ms), `params[1]` = 3.
5. `SFCFF.exe p <temp>\gamedata.cff t2054` then `... t2002`; on any `p` error
   keep the live file untouched and restore the backup.
6. Verified copy → `<root>\data\gamedata.cff` (owner step).
7. Remove the old wpn_spd data (SpellLine 242) if present on the installation
   — owner step; the old semantics are incompatible and are not reused.

## Lua (loose-override)

`object\object_effect_register.lua` is replaced **whole-file**. The prepared
file in the task temp is based on the live installation's file (it already
carries prior additions) plus one registration added after the Berserk line:

```lua
SpellEffect{line=251, hit="DecalBlessing", cast="ResolveYellow", castsound="spell_melee_heal", resolve="ResolveYellow"}
```

FX names are verified vanilla (used by the Berserk/Blessing ability lines in
`modding\Original Scripts\object\object_effect_register.lua`). Final FX
composition is an owner-open item; **without lua the mechanism still works**
(smoke S1 passes with no buff visual).

## Framework contracts (modder notes)

- Do NOT register damage-transform handlers in the `RANGED_SHOT` phase: the
  dispatcher ignores the handler's return there, while the handler's side
  effects apply on every shot.
- Phase dispatch order: the LAST registered `RANGED_SHOT` handler of a line
  runs FIRST (reverse registration order).
- Future-start procs are bounded by the 16-bit short window: the guard
  compares the clone's total arrival delta (launch spread within the window
  + flight) against `0x8000`. The guard is defensive only — launch spread is
  at most 6 ticks and the flight at most 30583 ticks, so the total stays
  below `0x8000` for any natural input; a fired guard would skip that
  arrow's enchantment with a warning ("RAPID FIRE: enchant delta overflow,
  proc skipped") while the arrow's physics stays untouched.

## Framework & mod install

1. Build the framework (`mingw32-make` from the SFSF root → `bin/sfsf.asi`)
   and install it per `data/developers/framework-structure.md` (loads at game
   start; `<root>\sfsf.log` appears).
2. Build the mod: `mingw32-make -C examples/rapid_fire` →
   `bin/rapid_fire.sfm`; install it into the SFSF mod directory per the
   framework structure doc.
3. The old wpn_spd build artifacts must not remain: `bin/rapid_fire.sfm` is
   rebuilt from the new source (registrations for lines 242/170 are gone).

## Granting the buff for smoke

The buff is the spell of SpellLine 251 — grant it to a shooter by any
conventional means available on the installation (spell cast via script/
console/item). The concrete delivery method is an owner-open item of the
smoke setup; the mod has no UI of its own and registers no items.

## Smoke checklist (run manually by the owner; live install
`B:\SteamLibrary\steamapps\common\Spellforce Platinum Edition\`)

- **S1 cascade (basic)**: a buffed shooter (params[1]=3) fires → the donor
  lands first, then exactly 3 extra arrows are LAUNCHED as a batch within the
  shot-completion animation window (~700 ms, evenly spread launch offsets)
  and land into the same victim keeping that spread (each clone flies the
  donor flight length from its own launch), each with the donor shot's
  snapshot damage; FPS/jitter observation of an army of buffed shooters vs
  unbuffed — into the smoke report (hot path).
- **S2 victim death**: the first arrow kills the victim → remaining clones are
  discarded by the engine without shooting; no crashes, no new errors
  in the log.
- **S2b shooter death**: the shooter dies with clones in flight → clones are
  discarded (owner 0xffff); actual behaviour into the smoke report; no crashes.
- **S3 params[1]=1**: data-only change → each shot yields exactly 1 extra
   arrow (data-driven).
- **S4 re-cast/refresh**: refresh prolongs the buff; clones already in flight
   from the previous window are not cancelled and land (supersession); no
  new errors.
- **S5 special arrows**: troll flamethrower/windarcher under the buff → clones
  carry SUBSPELL_ID (burn/visual; verifies the dispatch point after hookA:502).
- **S6 0-0 weapon**: the cascade lives, no damage, DealDamage(0) is skipped
  (hookP:186-189); no new errors in the log.
- **S7 critical strike**: 0x7fff donor damage → no volley (handler guard),
  donor behaviour is stock.
- **S8 slots mid-loop**: all 1999 effect slots taken (stress setup) → warning
  "no free effect slot", the loop stops at the first refusal, spawned clones
  keep flying, the donor is stock.
- **S9 contour regression (mandatory)**: (a) the existing 8 on-hit handlers
  (warcry/endurance/berserk/durability/trueshot/critical_hits — melee and
  ranged carriers, assistance, riposte) behave as before the change;
  (b) arrow_reflection and the vanilla rains (blizzard/rain_of_fire) behave as
  before; reflection on the victim + buff on the shooter: every arrival of a
  cascade clone is reflected by the stock hookP — the actual reflected count
  goes into the smoke report.
- **S10 ids/overwrite**: line 251/job 177/spell (3555→owner-confirmed id) do
  not conflict; no old wpn_spd left in build or data (`bin/rapid_fire.sfm`
  rebuilt from the new code, no 242/170 registrations, SpellLine 242 removed
  by the owner); template/TestMod untouched.
- **S11 render observation (variant A validation)**: record the actual look of
   waiting clones (standing at the t=0 point, direction gap) — photo/
  description into the smoke report; no render gate is built.
- **S12 enchant procs on arrival via future-start (v2.4)**: a shooter with
  enchanted weapon (noticeable chances) under the buff (params[1]≥2) fires →
  (a) the original arrow procs AT THE SHOT (vanilla block — immediate
  addSpell, coexistence unchanged); (b) every clone's proc INSTANCES are
  observable on the victim FROM THE SHOT on (they sit through the whole
  flight), the proc STATUS lands in the clone's arrival tick (clone arrival =
  its launch offset within the shot window + flight; Δ-countdown);
  rolls are independent per arrow, up to 3 procs per arrow (full vanilla
  roll); (c) the victim dies before a clone arrives → its instances are
  removed, no procs, no errors; (d) a dispel on the victim during the flight
  removes the pending procs (new behaviour, owner-accepted); (e) a special
  arrow (troll) under the buff → the clone's special subspell works
  (SUBSPELL_ID not taken), procs apply on top; (f) foreign projectiles (a
  plain arrow without the buff, spell projectiles) behave as before (no
  handler — nothing dispatched for them); (g) hot-path observation: FPS of a
  buffed shooter army vs S1.
