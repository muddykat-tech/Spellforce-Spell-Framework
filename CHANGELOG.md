# Changelog

## Unreleased

### Breaking

- Toolbox API function `addSpellToFigure` renamed to `attachSpellToFigure`: mods must update their `toolboxAPI->addSpellToFigure(...)` calls. The freed name `addSpellToFigure` is now a SpellFunctions wrapper that starts a spell between two figures from a pair of figure indices; entity types are always 1 (figure, implied by the name), positional entity types (4/5) need `addSpell` directly. The wrapper takes no start tick - the spell always starts at the current game tick (built-in signature `(SF_CGdSpell *, spell_id, source_index, target_index)`); future starts are only possible through `addSpell` directly.
- Figure API list accessors `getHealersList` / `disposeHealerList` renamed to `getIncomingSpellList` / `releaseIncomingSpellList`: mods must update their `figureAPI` calls. The list itself carries the in-flight spell casts (any spell line) against a figure as {caster figure id, spell id} pairs; the snapshot ownership and release semantics are unchanged, only the names.

### Fixed

- The pending-heal walk-phase scan counts healers walking to ground-cast healing spells (area/sentinel healing) again: for those casts the walk job carries a world point (entity types 4/5, entity index 0) instead of the patient figure, so the walker is matched by its healing job action alone; a figure target that is not the scanned patient and building/object targets (entity types 2/3) stay excluded.
- Aura of Fast Walking (spell line 113) applies to non-warrior allies again, vanilla parity; all other allied auras remain warriors-only.
- The incoming spell list snapshot is owned inside the wrappers: `sumIncomingHealFromList` takes the snapshot internally and releases it exactly once on its single return path, so no caller can leak it (`healing_ai_handler` leaked the snapshot on 2 of 3 paths before the wrappers took over the ownership).

### Changed

- The healing aura (AuraHealing line 103) survives the battle-mode teardown while it is still needed: the `figureTurnOffBattleMode` vanilla-fix hook collects line 103 for ending only when no enemies remain AND no ally is missing HP (the AI battle-data aggregate) - so after a battle with wounded allies the aura keeps ticking and heals the crowd, and once everyone is healthy it is torn down and cannot be re-cast (the AI aggregate gate sees zero missing HP). This replaces the previous unconditional skip of line 103 and breaks the original "end-aura -> reset `hasSpellOnIt` -> AI re-rank -> re-cast" loop in both states. This is a deliberate divergence from vanilla - vanilla `figure_turn_off_battle_mode` ended every aura on the figure once no enemies remain (the gate lives as a static helper inside the `figureTurnOffBattleMode` hook TU); every other aura-tagged and siege-aura-tagged line keeps the vanilla cleanup semantics unchanged, and the condition is deterministic (spell line is a static gamedata constant; enemy count and the missing-HP aggregate are lockstep sim state, MP-safe).
- Healing Aura (AuraHealing line 103) picks its heal target by the largest effective deficit - missing HP minus all incoming healing (the in-flight tagged list snapshot plus the pending walking-casts scan, saturating subtraction) - among candidates whose effective deficit reaches the sub-spell heal volume (`params[0]` of the line 166 sub-spell). This replaces the AI handler's first-found candidate loop (raw missing only) and the tick's enemy-proximity x health-percent pick with one formula shared by both points (a deliberate divergence from vanilla's lowest-health-percent tick pick); the AI rank is 0 when no ally clears the floor, the aura tick still applies the sub-spell to exactly one target per tick, and the per-tick mana charge is unchanged. The temporary `SF_OVERHEAL_DEBUG` diagnostic logging used to smoke-test these gates was removed after the battle smoke, so the shipped build carries no heal-decision log lines. The aura AI handler's success rank is 2 instead of vanilla 1 (2 unblocks the sticky selection floor - contest needs rank composite above the persisted ranking); refusals stay rank 0.

### Added

- `HEALING_SPELL` spell tag (`SpellTag` 0x800). Tagged vanilla spell lines: Healing (2), Sentinel Healing (44), Greater Healing (45), Suicide Heal (105), Healing Tower (144), Healing Aura (166); the aura wrapper line AuraHealing (103) is intentionally not tagged (it carries no heal payload itself - its sub-spells of line 166 do).
- Incoming-healing wrappers `sumIncomingHealFromList` / `sumIncomingPendingHeals` / `sumIncomingPendingHealsAllFigures` (internal, not published in the mod API tables). `sumIncomingPendingHeals` returns the total incoming healing of a target: the in-flight list pairs - counted honestly, only `HEALING_SPELL` tagged lines at each pair's own `params[0]` (a flying damage spell is not incoming healing) - plus allies currently walking to cast healing spells (jobs WalkToTarget 39 / ManualWalkToTarget 125 / PreCast 103), the window between the AI decision and the cast start that the list cannot see. `sumIncomingHealFromList` is the list part alone (snapshot owned internally); `sumIncomingPendingHealsAllFigures` is the roster-free walk-phase variant.
- Healing Aura (AuraHealing line 103) candidate check now counts all incoming healing of a candidate - the in-flight tagged pairs snapshot plus the pending walking casts scan - instead of only the sub-spell heal amount, so the aura stops stacking on targets that are already being healed.
- `healing_ai_handler` usefulness gate (a deliberate divergence from vanilla): the single-target healer refuses a target (rank 0) while its wound is below the smaller of max health / 10 and the spell's heal volume / 2 (both floored, HP units); an equal wound still triggers. The gate is additional to the vanilla full-health check and the rank 1/2 priority logic is untouched; heal volumes of 1 HP or less keep the gate passive. The healing aura is intentionally left without the gate (its ticks are small and infrequent).

## 5.0.0-RC

### Breaking

- Removed the hero registration API: `registerHero`, `linkHeroJSON` and the `SFHero` structure no longer exist in `registrationAPI`. Custom heroes are data-driven instead - JSON files in `sfsf\heroes\` (previously `sfsf\<mod id>\heroes\` linked from the mod).
- `getChanceToResistSpell` final parameter changed from `SF_SpellEffectInfo effect_info` to `uint16_t spell_index`.
- `createModInfo` signature changed from `char *mod_version` to `const char *mod_version`.
- Struct layouts changed in `sf_general_structures.h`. Mods built against 4.0.2 headers must be recompiled:
  - `CGdControllerClientData` — `AutoClass72` and `CGdEventIterator` grew from 5 to 8 entries (+24 bytes).
  - `CAppMenu_data` — remapped from `data[0x28]` blocks into named fields.
  - `SFSF_ModlistStruct` — added `author_label` and `type_label`.
  - `CMnuBase_data` — `flags` split out of `data[0x118]`.
  - `CUiMain_data` — `CGdMain`, `AC95`, `AC101`, `campaign_type` and `game_info` named.
  - `CAppMain_data` — `CAppSession` typed.

### Added

- Learnable abilities via JSON: `sfsf\abilities\*.json` files hold an array of `{skill, spec, level, ability_id}` records feeding the shared level-up registry. Records with `skill == 0 && spec == 0` are avatar level-up records (`level` = avatar level), `ability_id == 0` disables an ability for the key. Files override vanilla entries; every override logs a conflict naming both sources. Registered by the Ability Learn Module.
- Custom heroes are data-driven: `sfsf\heroes\*.json` (schema unchanged: `creo_id` + `gear`, max 7 entries). Files override vanilla heroes the same way - an empty `gear` list clears the gear of an already registered hero. Registered by the Custom Hero Module.
- Custom campaigns. Campaigns are defined by JSON files in `sfsf\campaigns\` and registered by the Custom Campaign Module. See README for the schema and rules.
- Custom campaign selection screen, reached from a `CUSTOM CAMPAIGNS` main menu button that appears only when at least one campaign is registered.
- Per-campaign save folders, created alongside the vanilla `save`, `temp`, `char`, `campaign2` and `campaign3` folders.
- Custom loading screens. `sfsf\screens.json` takes a map name and assigns a `.msb` mesh under `mesh\` to it, requires `.dds` texture in `textures\` of the same name.
- Mod conflict and error registry (`sf_error_registry`). Claims are tracked per domain: spell ID, spell effect ID, building ID, campaign folder, campaign name, campaign avatar type. Conflicts are attributed to the mod that lost the ID.
- Mod list rework: 11 rows per page with paging, and a detail panel showing name, version, author, mod type tag, description and errors. The framework and core modules are now listed alongside external mods.
- `illusion_ai_handler`, linked to `kGdSpellLineSelfIllusion`.
- `toolboxAPI.getSightRange`.
- `log_warning_level(DebugLevel level, ...)`.
- uiAPI functions: `containerAddControl`, `menuLabelConstructor`, `initMenuElement`, `getFonts`, `getFont`, `menuLabelSetFont`, `menuLabelSetString`, `newOperator`, `setScreenName`, `setCanFocus`, `attachControlToScreen`, `bringToFront`, `vfunction12`, `vfunction163`, `vfunction175`, `vfunction187`, `vfunction207`. - rename pending
- uiAPI `SF_String` functions: `SFprintf`, `SFStringConcat`, `SFStringConcatMulti`, `SFStringSetLength`, `SFStringFromWchar`, `SFStringDestructor`, `SFStringCopy`, `SFStringDeepCopy`, `SFStringCMbStr`, `SFStringConstructor`, `SFStringConstructor_char`, `SFStringConstructor_wchar`.
- Structures with probably correct annotations but use at your own risk: `GdAvatarInternal`, `GdAvatarData`, `GdAvatar`, `AutoClass82`, `SF_GameInfo`,
- Annotation of Structures for `SF_CGdMain`, `CGdMain_data`, `CAppSession`, `CMnuScreen`, `CUtlConfigFile`.
- `DllMain` reports init and version-check failures through `OutputDebugStringA`.

### Changed

- Full rewrite of vanilla game logic for function `CAppMenu::PrepareNewGame`
- Full rewrite of vanilla game logic for function `initFirstMap`: custom campaign folder and map selection, per-campaign starter kit override.
- Full rewrite of vanilla game logic for functions `loadQuickSave` and `quickLoad_helper`.
- Full rewrite of vanilla game logic for function `getSavePath`, resolving custom campaign save folders.
- Full rewrite of vanilla game logic for function `checkDirs`.
- `SF_String` constructors moved from `sf_vanilla_fix_hook.c` to `sf_ui_wrappers.c` and exposed through uiAPI. All internal call sites now go through uiAPI.
- Spell and building conflict checking moved from per-function local maps to the shared error registry.
- Handler replacement and on-hit registration messages moved to `log_debug` / `log_warning_level` at `DEBUG_LOW`.

### Fixed

- `stone_rain_phys_handler` indexed `SF_CGdWorld->unknown1` with a stride of 7 instead of the loop counter.
- `healing_aura_ai_handler` now requires an ally missing at least the sub-spell's heal amount before ranking the cast.
- `effect_self_illusion` now works on everyone, not avatar only.
- `createModInfo` did not check its allocation before writing, and used hardcoded field lengths instead of `sizeof`.

## 4.0.2-RC

### Fixed

- Visual effects from auras not disappearing after the aura ended.
- Incorrect damage calculations in the hit logic override.
- Vanilla unit overlay showing incorrect enchantment levels on rune army units.

### Added

- Foundational support for multiple auras running simultaneously.
