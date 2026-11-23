# mages_orb — data-driven weapon type sample

Type 22 "1HOrb" (JSON catalog `weapon_types/mages_orb.json`).

## Projectile FX (object_effect_register.lua)

```lua
RegisterEffect(kGdEffectProjectile, 22 , EffectGet("OrbIce"))   -- weapon-type: all units with id <= 1223
RegisterEffect(kGdEffectProjectile, <UnitID> , EffectGet(...))  -- per-unit: with the hook — any id, without the hook — only id > 1223
RegisterEffect(kGdEffectProjectile, <ItemID> , EffectGet(...))  -- avatar/hero (resolved by item)
```

- Add your own lines before the vanilla `RegisterEffect(kGdEffectProjectile, 0, Arrow)`.
- **Do not register item-keyed slots with id < 1223**: after the hook
  sf_template_resolver_hook they are intercepted by units with the same unit_data_id
  (collisions exist: unique-NPC 694-723, Troll Hurler 1217).
