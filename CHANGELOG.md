### Changelog Summary

**New Features**
- Created new examples: `aoe_freeaim.cpp`, `iceblade.cpp`, `arrow_reflection.cpp`
- Added support for AI spell handling.
- Implemented a spelltag system that allows mod developers to tag their spells in a manner that Spellforce will understand.
     - An example, if you create a combat ability, and you need units to treat it as such, you can apply a spell tag to do so.
- Created a new Mod Menu with the functionality to showcase installed mods.

**Bug Fixes**
- Fixed various typos and formatting issues
- Resolved compilation errors and redundant code handling
- Addressed specific bugs related to black towers AI and spell handling
- Corrected iterator usage and logical errors in handlers

**AI Enhancements**
- Fixed vanilla AI Bugs, as an example the Aura of Light, which was uncastable by player controlled units.
     - It was uncastable because only undead units were able to cast the aura of light; which is quite ironic.

**Miscellaneous**
- Improved documentation, including Doxygen and README.