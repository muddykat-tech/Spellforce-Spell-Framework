#include "SpellforceHandler.h"
#include "hooks.h"
#include "SpellforceSpells.h" // May be redundent as it is called inside (hooks.h -> SpellforceSpellFramework.h)

void initSpellMap()
{
	// Setup Vanilla Spell Handling!
	addSpellHandler(1, &fireburst_handler);
    addSpellHandler(0x9f, &fireburst_handler);
    addSpellHandler(0xea, &fireburst_handler);
}

void __thiscall fireburst_handler(SF_CGdSpell* _this, uint16_t spell_index) {
    _this->active_spell_list[spell_index].spell_job = 0x1;
    setXData(_this, spell_index, 0x12, 0);
    setXData(_this, spell_index, 0x26, 0);
}