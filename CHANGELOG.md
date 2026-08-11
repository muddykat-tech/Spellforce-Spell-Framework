# Changelog

## 5.0.0-RC

### Breaking

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
