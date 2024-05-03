#include "sf_spellhandler.h"
#include "sf_hooks.h"
#include "sf_registry.h"

void __thiscall default_handler (SF_CGdSpell * spell, uint16_t spell_index)
{
	;
}

void __thiscall fireburst_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0x1;
    setXData(_this, spell_index, SPELL_TICK_COUNT, 0);
    setXData(_this, spell_index, SPELL_DOUBLE_DAMAGE, 0);
}

void __thiscall icestrike_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0x8;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
    setXData(_this, spell_index, SPELL_TICK_COUNT, 0);
    setXData(_this, spell_index, SPELL_DOUBLE_DAMAGE, 0);
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
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall poison_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4;
	setXData(_this, spell_index, SPELL_TICK_COUNT, 0);
	setXData(_this, spell_index, SPELL_DOUBLE_DAMAGE, 0);
}

void __thiscall invulnerability_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall cure_poison_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5;
}

void __thiscall freeze_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xc;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall fog_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall illuminate_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x10;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall fireshield_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x12;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall fireball_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x11;
}


void __thiscall iceshield_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x13;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall decay1_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xd;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall decay2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x15;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall pain_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xe;
}


void __thiscall lifetap_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x14;
	setXData(_this, spell_index, SPELL_DOUBLE_DAMAGE, 0);
}

void __thiscall summons_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x17;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall hypnotize_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xf;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall iceshield2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x12;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall pestilence_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x19;
    setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
    setXData(_this, spell_index, SPELL_PESTILENCE_DAMAGE, 0);
    setXData(_this, spell_index, SPELL_DOUBLE_DAMAGE, 0);
}

void __thiscall cure_disease_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1a;
}

void __thiscall petrify_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x18;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall area_pain_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1b;
}

void __thiscall raise_dead_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x34;
}

void __thiscall death_grasp_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1c;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall inflexibility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1d;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall weaken_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1f;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall dark_banishing_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x20;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall area_slowness_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x21;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall area_inflexibility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x22;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall area_weaken_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x23;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall area_plague_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x24;
	setXData(_this, spell_index, SPELL_DOUBLE_DAMAGE, 0);
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall remediless_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x25;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall area_healing_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x26;
}

void __thiscall sentinel_healing_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x27;
}

void __thiscall charm_animal_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x45;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall thorn_shield_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2a;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall extinct_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x47;
}

void __thiscall detect_metal_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x64;
}

void __thiscall detect_magic_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1e;
}

void __thiscall unkn_spell_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall invisibility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x46;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}


void __thiscall stone_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x10;
}

void __thiscall aura_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x49;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
	setXData(_this, spell_index, EFFECT_EFFECT_INDEX, 0);
}

void __thiscall suicide_death_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4d;
}

void __thiscall feign_death_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7a;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall dispel_white_aura_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x52;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall suffocation_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4a;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall inablility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4c;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall slow_fighting_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4b;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall suicide_heal_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x51;
}

void __thiscall dominate_animal_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x54;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}

void __thiscall dispel_black_aura_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x53;
	setXData(_this, spell_index, SPELL_TICK_COUNT_AUX, 0);
}


void initSpellMap()
{
	// Setup Vanilla Spell Handling!
	addSpellHandler(0x01, &fireburst_handler);
	addSpellHandler(0x02, &healing_handler);
	addSpellHandler(0x03, &death_handler);
	addSpellHandler(0x04, &slowness_handler);
	addSpellHandler(0x05, &poison_handler);
	addSpellHandler(0x06, &invulnerability_handler);
	addSpellHandler(0x07, &cure_poison_handler);
	addSpellHandler(0x08, &default_handler); //empty spell line slot
	addSpellHandler(0x09, &freeze_handler);
	addSpellHandler(0x0a, &fog_handler);
	addSpellHandler(0x0b, &illuminate_handler);
	addSpellHandler(0x0c, &fireshield_handler);
	addSpellHandler(0x0d, &fireball_handler);
	addSpellHandler(0x0e, &icestrike_handler);
	addSpellHandler(0x0f, &iceshield_handler);

	addSpellHandler(0x10, &decay1_handler);
	addSpellHandler(0x11, &decay2_handler);
	addSpellHandler(0x12, &pain_handler);
	addSpellHandler(0x13, &lifetap_handler);
	addSpellHandler(0x14, &summons_handler);
	addSpellHandler(0x15, &hypnotize_handler);
	addSpellHandler(0x16, &iceshield2_handler);
	addSpellHandler(0x17, &pestilence_handler);
	addSpellHandler(0x18, &cure_disease_handler);
	addSpellHandler(0x19, &petrify_handler);
	addSpellHandler(0x1a, &default_handler); //empty spell line slot
	addSpellHandler(0x1b, &default_handler); //empty spell line slot
	addSpellHandler(0x1c, &area_pain_handler);
	addSpellHandler(0x1d, &summons_handler);
	addSpellHandler(0x1e, &raise_dead_handler);
	addSpellHandler(0x1f, &summons_handler);

	addSpellHandler(0x20, &death_grasp_handler);
	addSpellHandler(0x21, &summons_handler);
	addSpellHandler(0x22, &inflexibility_handler);
	addSpellHandler(0x23, &weaken_handler);
	addSpellHandler(0x24, &dark_banishing_handler);
	addSpellHandler(0x25, &area_slowness_handler);
	addSpellHandler(0x26, &area_inflexibility_handler);
	addSpellHandler(0x27, &area_weaken_handler);
	addSpellHandler(0x28, &area_plague_handler);
	addSpellHandler(0x29, &remediless_handler);
	addSpellHandler(0x2a, &default_handler); //dark might
	addSpellHandler(0x2b, &area_healing_handler);
	addSpellHandler(0x2c, &sentinel_healing_handler);
	addSpellHandler(0x2d, &healing_handler);
	addSpellHandler(0x2e, &charm_animal_handler);
	addSpellHandler(0x2f, &thorn_shield_handler);

	addSpellHandler(0x50, &default_handler); //empty spell line slot
	addSpellHandler(0x51, &extinct_handler);
	addSpellHandler(0x52, &detect_metal_handler);
	addSpellHandler(0x53, &detect_magic_handler);
	addSpellHandler(0x54, &default_handler); //unnamed spell 84
	addSpellHandler(0x55, &unkn_spell_handler); //unnamed spell 85
	addSpellHandler(0x56, &invisibility_handler);
	addSpellHandler(0x57, &stone_handler);
	addSpellHandler(0x58, &aura_handler);
	addSpellHandler(0x59, &aura_handler);
	addSpellHandler(0x5a, &suicide_death_handler);
	addSpellHandler(0x5b, &aura_handler);
	addSpellHandler(0x5c, &summons_handler);
	addSpellHandler(0x5d, &feign_death_handler);
	addSpellHandler(0x5e, &aura_handler);
	addSpellHandler(0x5f, &aura_handler);
	
	addSpellHandler(0x60, &dispel_white_aura_handler)
	addSpellHandler(0x61, &aura_handler);
	addSpellHandler(0x62, &aura_handler);
	addSpellHandler(0x63, &suffocation_handler);
	addSpellHandler(0x64, &inablility_handler);
	addSpellHandler(0x65, &slow_fighting_handler);
	addSpellHandler(0x66, &aura_handler);
	addSpellHandler(0x67, &aura_handler);
	addSpellHandler(0x68, &aura_handler);
	addSpellHandler(0x69, &suicide_heal_handler);
	addSpellHandler(0x6a, &summons_handler);
	addSpellHandler(0x6b, &aura_handler);
	addSpellHandler(0x6c, &dominate_animal_handler);
	addSpellHandler(0x6d, &summons_handler);
	addSpellHandler(0x6e, &aura_handler);
	addSpellHandler(0x6f, &aura_handler);


	addSpellHandler(0x70, &dispel_black_aura_handler);
	addSpellHandler(0x71, &aura_handler);
	addSpellHandler(0x72, &aura_handler);
	addSpellHandler(0x73, &aura_handler);

	addSpellHandler(0x79, &default_handler); //empty spell line slot
	addSpellHandler(0x7f, &aura_handler);

	addSpellHandler(0x81, &aura_handler);
	addSpellHandler(0x83, &aura_handler);
	
	addSpellHandler(0x85, &summons_handler);
	addSpellHandler(0x88, &summons_handler);
	addSpellHandler(0x8d, &summons_handler);

	addSpellHandler(0x9f, &fireburst_handler);

	addSpellHandler(0xa6, &healing_handler);
	addSpellHandler(0xa7, &hypnotize_handler);

    addSpellHandler(0xbc, &summons_handler);
	
	addSpellHandler(0xc0, &aura_handler);
    addSpellHandler(0xc6, &summons_handler);
    addSpellHandler(0xcd, &summons_handler);
    addSpellHandler(0xce, &summons_handler);

    addSpellHandler(0xd1, &summons_handler);
	addSpellHandler(0xdf, &aura_handler);

	addSpellHandler(0xe0, &default_handler); //empty spell line slot
	addSpellHandler(0xe1, &aura_handler);
	addSpellHandler(0xe2, &aura_handler);
	addSpellHandler(0xe3, &aura_handler);
	addSpellHandler(0xe4, &aura_handler);

    addSpellHandler(0xe7, &lifetap_handler);
    addSpellHandler(0xea, &fireburst_handler);
    addSpellHandler(0xeb, &icestrike_handler);
    addSpellHandler(0xef, &fireball_handler);

    addSpellHandler(0xf0, &pain_handler);

}