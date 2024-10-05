# Overview

- Added support for On-Hit Effects! 
- Added structures for interfacing with game world (CGdWorld & CGdWorldToolbox)
- Added functions for summoning creatures (addUnit & findClosestFreePosition & setTask)
- Added function RemoveSpellFrom list (useful in group spells situation)
- Fixed conflict detection. Now framework can tell apart vanilla override and conflicts
- Properly passed through logic for dealDamage handlers
- Fixed bug with spell refresh handler registration
- Provided examples for interaction with most parts of the framework (persistent_damage, shield_wall, interferrence)
- Provided example for interaction with world (controlled_summons)
- Provided enum with vanilla spell lines
- Provided usefull spell flags