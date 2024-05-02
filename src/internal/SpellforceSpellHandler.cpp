#include "SpellforceHandler.h"
#include "hooks.h"
#include "SpellforceSpells.h" // May be redundent as it is called inside (hooks.h -> SpellforceSpellFramework.h)

void initSpellMap()
{
	// Setup Vanilla Spell Handling!
	addSpellHandler(0x01, &fireburst_handler);
	addSpellHandler(0x02, &healing_handler);
	addSpellHandler(0x03, &death_handler);
	addSpellHandler(0x04, &slowness_handler);
	addSpellHandler(0x05, &poison_handler);

	addSpellHandler(0x0e, &icestrike_handler);
	addSpellHandler(0x2d, &healing_handler);
	addSpellHandler(0xa6, &healing_handler);
    addSpellHandler(0x9f, &fireburst_handler);
    addSpellHandler(0xea, &fireburst_handler);
    addSpellHandler(0xeb, &icestrike_handler);
}

void __thiscall fireburst_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0x1;
    setXData(_this, spell_index, 0x12, 0);
    setXData(_this, spell_index, 0x26, 0);
}

void __thiscall icestrike_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0x8;
    setXData(_this, spell_index, 0x05, 0);
    setXData(_this, spell_index, 0x12, 0);
    setXData(_this, spell_index, 0x26, 0);
}

void __thiscall healing_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2;
}

void __thiscall death_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3;
}

void __thiscall slowness_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xb;
    setXData(_this, spell_index, 0x05, 0);
}

void __thiscall poison_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4;
	setXData(_this, spell_index, 0x12, 0);
	setXData(_this, spell_index, 0x26, 0);
}