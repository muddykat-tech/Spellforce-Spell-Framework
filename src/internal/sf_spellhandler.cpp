#include "sf_spellhandler.h"
#include "sf_hooks.h"
#include "sf_registry.h"

void __thiscall initializeSpellData(SF_CGdSpell* _this, uint16_t spell_id, SpellDataKey key) {
	setXData(_this, spell_id, key, 0);
}

void __thiscall default_handler (SF_CGdSpell * spell, uint16_t spell_index)
{
	;
}

void __thiscall fireburst_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0x1;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
    initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall icestrike_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
    _this->active_spell_list[spell_index].spell_job = 0x8;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
    initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
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
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall poison_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall invulnerability_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall cure_poison_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5;
}

void __thiscall freeze_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xc;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall fog_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall illuminate_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x10;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall fireshield_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x12;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall fireball_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x11;
}

void __thiscall iceshield_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x13;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall decay1_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xd;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall decay2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x15;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall pain_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xe;
}

void __thiscall lifetap_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x14;
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall summons_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x17;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall hypnotize_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xf;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall iceshield2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x12;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall pestilence_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x19;
    initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
    initializeSpellData(_this, spell_index, SPELL_PESTILENCE_DAMAGE);
    initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall cure_disease_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1a;
}

void __thiscall petrify_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x18;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
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
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall inflexibility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall weaken_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall dark_banishing_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x20;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_slowness_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x21;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_inflexibility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x22;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_weaken_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x23;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_plague_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x24;
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall remediless_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x25;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
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
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall thorn_shield_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall quickness_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x28;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall area_quickness_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x29;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall flexibility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall area_flexibility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall strength_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall area_strength_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall guard_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall remove_curse_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x48;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall regenerate_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x30;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall hallow_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x31;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall fireshield2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x32;
}

void __thiscall thorn_shield2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x33;
}

void __thiscall forget_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x37;
}

void __thiscall self_illusion_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x68;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall retention_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x38;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall brilliance_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x39;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall sacrifice_mana_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3a;
}

void __thiscall manatap_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3b;
}

void __thiscall manadrain_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3c;
}

void __thiscall shock_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x42;
}

void __thiscall disrupt_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3f;
}

void __thiscall confuse_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall rain_of_fire_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x41;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall blizzard_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x43;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall acid_cloud_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x40;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall stone_rain_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x44;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall amok_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
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
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall invisibility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x46;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall stone_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x10;
}

void __thiscall aura_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x49;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, EFFECT_EFFECT_INDEX);
}

void __thiscall suicide_death_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4d;
}

void __thiscall feign_death_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall dispel_white_aura_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x52;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall suffocation_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall inablility_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall slow_fighting_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall suicide_heal_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x51;
}

void __thiscall dominate_animal_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x54;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall dispel_black_aura_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x53;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall dexterity_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x50;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall edurance_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall fast_fighting_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall distract_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x63;
}

void __thiscall dominate_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall charm_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall befriend_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5d;
}

void __thiscall disenchant_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7c;
}

void __thiscall charisma_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x59;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall shockwave_handler(SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5c;
}

void __thiscall demoralization_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x75;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall enlightenment_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER2);
}

void __thiscall meditation_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5b;
}

void __thiscall wave_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x55;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, EFFECT_EFFECT_INDEX);
}


void __thiscall melt_resistance_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x58;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall chill_resistance_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x57;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall rock_bullet_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x56;
	setXData(_this, spell_index, 0x45, 0);
}

void __thiscall conservation_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x65;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_CONSERVATION_SHIELD);
}

void __thiscall tower_arrow_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x60;
}

void __thiscall tower_healing_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x61;
}

void __thiscall tower_icestrike_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x62;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
}

void __thiscall lifetap_aura_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x66;
}

void __thiscall fireball2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x67;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall ability_warcry_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x69;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_benefactions_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_patronize_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_endurance_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_berserk_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_boons_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_shelter_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_durability_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x70;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_trueshot_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x71;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_steelskin_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x72;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_salvo_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x73;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall spark_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x74;
}

void __thiscall tower_hypnotize_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x76;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall tower_pain_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x77;
}

void __thiscall tower_stone_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x78;
}

void __thiscall cloak_of_nor_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x79;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall unkn_spell2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}


void __thiscall freeze2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall freeze3_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall lava_bullet_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x80;
}

void __thiscall tower_extinct_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x81;
}

void __thiscall manatap_aura_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x82;
}

void __thiscall firebane_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x83;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall black_essence_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x84;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall white_essence_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x86;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}


void __thiscall chain_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa2;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall eternity_aura_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x91;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER2);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER3);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER4);
}

void __thiscall elemental_essence_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x88;
}

void __thiscall mental_essence_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8a;
}

void __thiscall black_almightness_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x85;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall white_almightness_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x87;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall elemental_almightness_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x89;
}

void __thiscall mental_almightness_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8b;
}

void __thiscall elemental_almightness2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall elemental_essence2_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall assistance_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall holy_touch_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8f;
}

void __thiscall revenge_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x97;
}

void __thiscall area_roots_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x90;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall roots_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa3;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall reinforcement_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x99;
}

void __thiscall cannibalize_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x92;
}

void __thiscall torture_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x98;
}

void __thiscall dominate_undead_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x93;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall mutation_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER2);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER3);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER4);
}

void __thiscall area_darkness_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x95;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_freeze_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x95;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall clay_feet_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall mirage_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa4;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall feedback_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa6;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_hypnotize_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_confuse_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall manashield_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa1;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}


void __thiscall shift_mana_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x96;
}

void __thiscall ability_shift_life_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9e;
}

void __thiscall ability_riposte_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}


void __thiscall ability_critical_hits_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa0;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}


void __thiscall belial_effect_handler (SF_CGdSpell* _this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa5;
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

	addSpellHandler(0x30, &quickness_handler);
	addSpellHandler(0x31, &area_quickness_handler);
	addSpellHandler(0x32, &flexibility_handler);
	addSpellHandler(0x33, &area_flexibility_handler);
	addSpellHandler(0x34, &strength_handler);
	addSpellHandler(0x35, &area_strength_handler);
	addSpellHandler(0x36, &guard_handler);
	addSpellHandler(0x37, &remove_curse_handler);
	addSpellHandler(0x38, &regenerate_handler);
	addSpellHandler(0x39, &default_handler); //holy might
	addSpellHandler(0x3a, &hallow_handler);
	addSpellHandler(0x3b, &default_handler); //empty slot
	addSpellHandler(0x3c, &fireshield2_handler); 
	addSpellHandler(0x3d, &thorn_shield2_handler);
	addSpellHandler(0x3e, &forget_handler); 
	addSpellHandler(0x3f, &self_illusion_handler); 

	addSpellHandler(0x40, &retention_handler);
	addSpellHandler(0x41, &brilliance_handler);
	addSpellHandler(0x42, &sacrifice_mana_handler);
	addSpellHandler(0x43, &manatap_handler);
	addSpellHandler(0x44, &manadrain_handler);
	addSpellHandler(0x45, &shock_handler);
	addSpellHandler(0x46, &disrupt_handler);
	addSpellHandler(0x47, &default_handler); //empty slot
	addSpellHandler(0x48, &confuse_handler);
	addSpellHandler(0x49, &rain_of_fire_handler);
	addSpellHandler(0x4a, &blizzard_handler);
	addSpellHandler(0x4b, &acid_cloud_handler);
	addSpellHandler(0x4c, &stone_rain_handler);
	addSpellHandler(0x4d, &default_handler); //empty slot
	addSpellHandler(0x4e, &default_handler); //empty slot
	addSpellHandler(0x4f, &amok_handler);

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
	
	addSpellHandler(0x60, &dispel_white_aura_handler);
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
	addSpellHandler(0x74, &dexterity_handler);
	addSpellHandler(0x75, &edurance_handler);
	addSpellHandler(0x76, &fast_fighting_handler);
	addSpellHandler(0x77, &distract_handler);
	addSpellHandler(0x78, &dominate_handler);
	addSpellHandler(0x79, &default_handler); //empty spell line slot
	addSpellHandler(0x7a, &charm_handler);
	addSpellHandler(0x7b, &befriend_handler);
	addSpellHandler(0x7c, &disenchant_handler);
	addSpellHandler(0x7d, &charisma_handler);
	addSpellHandler(0x7e, &shockwave_handler);
	addSpellHandler(0x7f, &aura_handler);

	addSpellHandler(0x80, &demoralization_handler);
	addSpellHandler(0x81, &aura_handler);
	addSpellHandler(0x82, &enlightenment_handler);
	addSpellHandler(0x83, &aura_handler);
	addSpellHandler(0x84, &meditation_handler);
	addSpellHandler(0x85, &summons_handler);
	addSpellHandler(0x86, &wave_handler);
	addSpellHandler(0x87, &melt_resistance_handler);
	addSpellHandler(0x88, &summons_handler);
	addSpellHandler(0x89, &wave_handler);
	addSpellHandler(0x8a, &chill_resistance_handler);
	addSpellHandler(0x8b, &rock_bullet_handler);
	addSpellHandler(0x8c, &conservation_handler);
	addSpellHandler(0x8d, &summons_handler);
	addSpellHandler(0x8e, &wave_handler);
	addSpellHandler(0x8f, &tower_arrow_handler);

	addSpellHandler(0x90, &tower_healing_handler);
	addSpellHandler(0x91, &tower_icestrike_handler);
	addSpellHandler(0x92, &lifetap_aura_handler);
	addSpellHandler(0x93, &fireball2_handler);
	addSpellHandler(0x94, &ability_warcry_handler);
	addSpellHandler(0x95, &ability_benefactions_handler);
	addSpellHandler(0x96, &ability_patronize_handler);
	addSpellHandler(0x97, &ability_endurance_handler);
	addSpellHandler(0x98, &ability_berserk_handler);
	addSpellHandler(0x99, &ability_boons_handler);
	addSpellHandler(0x9a, &ability_shelter_handler);
	addSpellHandler(0x9b, &ability_durability_handler);
	addSpellHandler(0x9c, &ability_trueshot_handler);
	addSpellHandler(0x9d, &ability_steelskin_handler);
	addSpellHandler(0x9e, &ability_salvo_handler);
	addSpellHandler(0x9f, &fireburst_handler);

	addSpellHandler(0xa0, &spark_handler);
	addSpellHandler(0xa1, &tower_hypnotize_handler);
	addSpellHandler(0xa2, &tower_pain_handler);
	addSpellHandler(0xa3, &tower_stone_handler);
	addSpellHandler(0xa4, &cloak_of_nor_handler);
	addSpellHandler(0xa5, &unkn_spell2_handler);
	addSpellHandler(0xa6, &healing_handler);
	addSpellHandler(0xa7, &hypnotize_handler);
	addSpellHandler(0xa8, &freeze2_handler);
	addSpellHandler(0xa9, &freeze3_handler);
	addSpellHandler(0xaa, &lava_bullet_handler);
	addSpellHandler(0xab, &tower_extinct_handler);
	addSpellHandler(0xac, &manatap_aura_handler);
	addSpellHandler(0xad, &firebane_handler);
	addSpellHandler(0xae, &black_essence_handler);
	addSpellHandler(0xaf, &white_essence_handler);

	addSpellHandler(0xb0, &elemental_essence_handler);
	addSpellHandler(0xb1, &mental_essence_handler);
	addSpellHandler(0xb2, &black_almightness_handler);
	addSpellHandler(0xb3, &white_almightness_handler);
	addSpellHandler(0xb4, &elemental_almightness_handler);
	addSpellHandler(0xb5, &mental_almightness_handler);
	addSpellHandler(0xb6, &elemental_almightness2_handler);
	addSpellHandler(0xb7, &elemental_essence2_handler);
	addSpellHandler(0xb8, &assistance_handler);
	addSpellHandler(0xb9, &holy_touch_handler);
	addSpellHandler(0xba, &revenge_handler);
	addSpellHandler(0xbb, &area_roots_handler);
    addSpellHandler(0xbc, &summons_handler);
	addSpellHandler(0xbd, &roots_handler);
	addSpellHandler(0xbe, &chain_handler);
	addSpellHandler(0xbf, &reinforcement_handler);

	addSpellHandler(0xc0, &aura_handler);
	addSpellHandler(0xc1, &chain_handler);
	addSpellHandler(0xc2, &cannibalize_handler);
	addSpellHandler(0xc3, &torture_handler);
	addSpellHandler(0xc4, &chain_handler);
    addSpellHandler(0xc5, &dominate_undead_handler);
    addSpellHandler(0xc6, &summons_handler);
    addSpellHandler(0xc7, &mutation_handler);
    addSpellHandler(0xc8, &area_darkness_handler);
    addSpellHandler(0xc9, &chain_handler);
    addSpellHandler(0xca, &chain_handler);
    addSpellHandler(0xcb, &summons_handler);
    addSpellHandler(0xcc, &chain_handler);
    addSpellHandler(0xcd, &chain_handler);
    addSpellHandler(0xce, &summons_handler);
    addSpellHandler(0xcf, &area_freeze_handler);

    addSpellHandler(0xd0, &chain_handler);
    addSpellHandler(0xd1, &summons_handler);
    addSpellHandler(0xd2, &clay_feet_handler);
    addSpellHandler(0xd3, &mirage_handler);
    addSpellHandler(0xd4, &chain_handler);
    addSpellHandler(0xd5, &feedback_handler);
    addSpellHandler(0xd6, &chain_handler);
    addSpellHandler(0xd7, &area_hypnotize_handler);
    addSpellHandler(0xd8, &area_confuse_handler);
    addSpellHandler(0xd9, &chain_handler);
    addSpellHandler(0xda, &manashield_handler);
	addSpellHandler(0xdb, &shift_mana_handler);
	addSpellHandler(0xdc, &ability_shift_life_handler);
	addSpellHandler(0xdd, &ability_riposte_handler);
	addSpellHandler(0xde, &ability_critical_hits_handler);
	addSpellHandler(0xdf, &aura_handler);

	addSpellHandler(0xe0, &default_handler); //empty spell line slot
	addSpellHandler(0xe1, &aura_handler);
	addSpellHandler(0xe2, &aura_handler);
	addSpellHandler(0xe3, &aura_handler);
	addSpellHandler(0xe4, &aura_handler);
 	addSpellHandler(0xe5, &eternity_aura_handler);
	addSpellHandler(0xe6, &hallow_handler);
    addSpellHandler(0xe7, &lifetap_handler);
	addSpellHandler(0xe8, &manatap_handler);
	addSpellHandler(0xe9, &mutation_handler);
    addSpellHandler(0xea, &fireburst_handler);
    addSpellHandler(0xeb, &icestrike_handler);
    addSpellHandler(0xec, &rock_bullet_handler);
    addSpellHandler(0xed, &charm_handler);
	addSpellHandler(0xee, &shock_handler);
    addSpellHandler(0xef, &fireball_handler);

    addSpellHandler(0xf0, &pain_handler);
    addSpellHandler(0xf1, &belial_effect_handler);

}