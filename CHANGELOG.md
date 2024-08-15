
# Overview
- List of the events, available for developer to put spell logic on:
- Spell application, 2 places
- One for the money spell logic selection and extra data setup, two for the  show  spell logic itself.
- Spell ending and refreshing on target
- Now you can prevent spell clipping last ticks of damage
- Now you can have you buffs and debuffs restore target to preexisting state
- Now you can have specific logic of overwriting instances of the same spell with different levels
- Spells sub effects
- Now you can implement rain, wave and chain spells of your liking
- Target gets damaged.
- Wanna have "shield wall", that substracts flat damage amount from damage taken? Now you can make it.
- A lot of API functions available for developer to mess with. 
- Documentation coming soon (TM)
  
<details>
  <summary>Commit Log</summary>
  
- Fixed rain of fire (UnSchtalch)
- Conservation handler fixed (UnSchtalch)
- Minor Fixes with Spell Registration ID Mismatches (muddykat-tech)
- Fireshield fix (UnSchtalch)
- On-hit hook disabled Fixed deal damage hook and some debug info (UnSchtalch)
- Fix wrong registration for fireshield (UnSchtalch)
- Makefile fix (UnSchtalch)
- Implemented Foundation for On Hit Hook (muddykat-tech)
- Hypnotize deal damage trigger register (UnSchtalch)
- Handler registration enabled. (UnSchtalch)
- Deal Damage handler registration. TODO: implement hypnotize handlers (UnSchtalch)
- Feign Death and Mana shield handlers done (UnSchtalch)
- Parsed through 'getManaCurrent, subMana, figureSetNewJob' Created a Wrapper to access Spell ID from Spell Index, Modified damage hook to pack spell index into the 2D (muddykat-tech)
- Death Grasp and feedback done Need passthrough to implement manashield Need passthrough to implement feign death (UnSchtalch)
- Conservation handler and fix for signature of FigureTryClearCheckSpellsBeforeJob2 function (UnSchtalch)
- Elemental and white magic shield handling (UnSchtalch)
- Work on dmg handlers started. Muddy, we DO need to pass spell_id down the line. (UnSchtalch)
- Second part of the deal damage fix (UnSchtalch)
- Fix return value for the deal damage hook (UnSchtalch)
- modified the damage handler input arguments to include the flags 'is_spell_damage', 'param_5', 'vry_unknown_6' (muddykat-tech)
- Implemented Deal Damage Phase System to allow the order of spell damage modification, untested (muddykat-tech)
- Merge branch 'master' of https://github.com/muddykat-tech/Spellforce-Spell-Framework (muddykat-tech)
- Menu Hook Attribute changed to include 'no_caller_saved_registers' (muddykat-tech)
- Fixed damage hook and multi-stage hooks crash (UnSchtalch)
- Menu hook fixed (UnSchtalch)
- Damage Hook Foundational Work (muddykat-tech)
- Damage hook compilation options (UnSchtalch)
- Merge branch 'master' of https://github.com/muddykat-tech/Spellforce-Spellframework.git (UnSchtalch)
- Damage hook itself is done, function with hook logic - is not. Correct addreses for enrty and return set up (UnSchtalch)
- Finished Changing Variable Convention to use g_ and s_ for global and static indications (muddykat-tech)
- Merge branch 'master' of https://github.com/muddykat-tech/Spellforce-Spell-Framework (muddykat-tech)
- Notating Global Variables with g_ prefix and Static Variables with s_ (WIP) (muddykat-tech)
- Refresh handlers register End spell bug fix typo fix (UnSchtalch)
- Finished Spelltype Registration with new Method (muddykat-tech)
- added more spells from vanilla to new registration method (muddykat-tech)
- Vanilla Spell Rework (muddykat-tech)
- separated console hook into it's own folder (muddykat-tech)
- Restructure of Internal Data Hook Initialization (muddykat-tech)
- Updated Comments (muddykat-tech)
- Fixed issues with makefile and git hook, no need for updating it now! (muddykat-tech)
- Separated Hooks into their own files, made a neat spinner for the make file (muddykat-tech)
- label location update (muddykat-tech)
- Cleaned up some logging functions and commented code (muddykat-tech)
- Cleaned up Menu Hook (muddykat-tech)
- More Polish to New Start Menu Label (muddykat-tech)
- Working Label Hook for Main Menu (muddykat-tech)
- Default handler fix (UnSchtalch)
- Update sf_spellrefresh_handlers.cpp (UnSchtalch)
- Cases 0x34..0x63 and bug fixes (UnSchtalch)
- Special refresh cases 0x04..0x32, renamed 0x17 to pestilence WARINING: Removed "onSpellRemove" call from the default case for 1st block. DO NOT PUT THIS IN PRODUCTION (UnSchtalch)
- Minor Refactor for Vanilla Registration, Domination Refresh Handlers Added (muddykat-tech)
- Refresh Handler Additions (muddykat-tech)
- Hook for Spell Refresh Complete, Handlers still WIP (muddykat-tech)
- Basic Foundation for Spell Refresh, not hooked yet (muddykat-tech)
- Pass through for  getSpellIndexOfType (UnSchtalch)
- Everything working, but debuff is not consumed. (UnSchtalch)
- Hook added Shit ain't working (UnSchtalch)
- Fixed Registration Issues with Sub Effects (muddykat-tech)
- Mostly passed through all stuff required (UnSchtalch)
- Added a few comments (muddykat-tech)
- Basic Work on Deal Damage Hook (muddykat-tech)
- Extra Comments in Damage Deal Hook (muddykat-tech)
- Hooked Deal Damage Func, Updated Variable in SF_CGDEffect to use SF_CGdSpell to avoid conversion warnings (muddykat-tech)
- Registry for multi-stage spells. Not tested (UnSchtalch)
- Common and elemental chain handlers. (UnSchtalch)
- Include effectAPI and handlers to be compilable (UnSchtalch)
- Multistage handling, step 2. Writing handlers. (UnSchtalch)
- Structures refactor, 1st step of multi-stage spells. (UnSchtalch)
- Add Spell to Figure function passthrough (UnSchtalch)
- Compile-time fix (UnSchtalch)
- Feet of clay registered too (UnSchtalch)
- Proper registration for block 1 end handlers (UnSchtalch)
- Futher Registration of Spell End Handlers (muddykat-tech)
- Fixed Minor Bug in Conflict Detection (muddykat-tech)
- Registered More Spell End Handlers (muddykat-tech)
- Registered More Spell End Handlers (muddykat-tech)
- Modified create-examples batch file to also move files to a single folder in root called 'examples-bin' (muddykat-tech)
- Added Hook Scripts to ensure project compiles before commit or push (muddykat-tech)
- Code Formatting (muddykat-tech)
- Improved Conflict Detection (muddykat-tech)
- Refactor of Mod Initialization and Registration (muddykat-tech)
- Very Large Registration System Rework, in progress, but functional (muddykat-tech)
- Minor Cleanup (muddykat-tech)
- Removed redundent wrappers for end spell handlers (muddykat-tech)
- Logging Structure Implemented (muddykat-tech)
- Another typo fix (UnSchtalch)
- Eternity handler. mutation left. (UnSchtalch)
- Typo in Fight speed fixed + feet of clay handler (D2) is done. Mutation and eternity. (UnSchtalch)
- Block one mostly done, eternity, feet of clay, mutation left (UnSchtalch)
- Implemented Function Parse and Sanitization for mod info (muddykat-tech)
- Implemented Mod Descriptors for easier debugging (muddykat-tech)
- Added end spell handlers for blocks 6 -> 0xda (muddykat-tech)
- Slow fighting and dexterity end handlers (UnSchtalch)
- Setup for End Handler Blocks - 0x6 -> 0xda (muddykat-tech)
- Exploration of GUI Functions, Rough idea of where to go, but not function at the moment (muddykat-tech)
- Dirty, but working Menu Trigger Hook (muddykat-tech)
- Menu Hook Testing (muddykat-tech)
- inablitiy handler (UnSchtalch)
- Implemented Mod Counting during Mod Loading Phase, can be used later for displaying mod information (muddykat-tech)
- Main Menu Loading Trigger Hooks (WIP) (muddykat-tech)
- Suffocation Handler (UnSchtalch)
- Finished Spellend Hook (muddykat-tech)
- removed redundent OutputDebugStringA calls inside log functions in wrappers (muddykat-tech)
- Working console print (UnSchtalch)
- spell_end hook setup (UnSchtalch)
- Console Log fix (UnSchtalch)
- Modified Spelltype Replacement Warning (muddykat-tech)
- Split out remaining wrapper functions from sf_hooks.c into sf_wrappers.c (muddykat-tech)
- Updated Examples and TestMod with API Changes (muddykat-tech)
- Internal Code Convention Rework (wip) (muddykat-tech)
- Internal Code Style Partial Refactor + Spell End Registration and Handlers (muddykat-tech)
- Fixed warning on signed / unsigned issue with addBonusMultToStatistic Wrapper (muddykat-tech)
- Internal Restructure and fixed some missing declare functions from merge (muddykat-tech)
- Merged Examples (muddykat-tech)
- Large API Refactor (muddykat-tech)
- Some more fixes in handlers Retention/Brilliance Handlers (UnSchtalch)
- Strength and agi buff spell end handlers (UnSchtalch)
- Quick patch (variable declaration) (muddykat-tech)
- Add quickness, weaken, inflex, quicken (UnSchtalch)
- Slowness and decay handler (UnSchtalch)
- Untested implementation of building iterator initial functions (init and SetPointers) (muddykat-tech)
- A bit more handling mess Gotta remove excessive handlers from end_registry.h tomorrow (UnSchtalch)
- Initial work on RefreshSpell and EndSpell handling (UnSchtalch)
- Fixed getXData usage. Now properly works (UnSchtalch)
- Basic Start for Building Iterator, WIP (muddykat-tech)
- Minor clean up for forward compat (UnSchtalch)
- Moved persisent aoe to new example, restored old code (UnSchtalch)
- Proper FX handling for targets hit (UnSchtalch)
- Corrected wrong function pointers (UnSchtalch)
- Fixed usage of wrong get/set/addTo functions in code (UnSchtalch)
- Fixed interval issues. Spell doesn't end (UnSchtalch)
- Partially working AoE persistent spell. Gotta fix spell ending and tick handling. Muddy, I do need your input here. (UnSchtalch)
- BuildingToolBox:DealDamage (UnSchtalch)
- Small cleanup (UnSchtalch)
- Wrapper Functions for Figure Iterator and Untested Dispose Function (muddykat-tech)
- Passed through check for friend or for Now targets are affected propely (UnSchtalch)
- Update api (UnSchtalch)
- AOE Spell working, but requires more fine-tunning (UnSchtalch)
- AOE spell complete. But not working (UnSchtalch)
- Fix null pointer crash Setup iteration through figures. (UnSchtalch)
- Re-ident + passthrough of CGdSpell::getTargetsRectangle (UnSchtalch)
- DOT spell effect testing (muddykat-tech)
- Reformat and iterator rewrite (UnSchtalch)
- Added Param Names to Declared functions (muddykat-tech)
- Improved SFSF Logging (muddykat-tech)
- Setup for AOE instant example (UnSchtalch)
</details>