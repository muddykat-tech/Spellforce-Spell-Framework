#include "sf_spelltype_handlers.h"
#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"
#include "../registry/sf_spelltype_registry.h"
#include "../registry/sf_registry.h"

void __thiscall default_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	;
}

void __thiscall fireburst_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall icestrike_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall healing_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2;
}

void __thiscall death_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3;
}

void __thiscall slowness_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xb;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall poison_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall invulnerability_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall cure_poison_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5;
}

void __thiscall freeze_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xc;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall fog_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall illuminate_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x10;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall fireshield_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x12;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall fireball_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x11;
}

void __thiscall iceshield_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x13;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall decay1_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xd;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall decay2_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x15;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall pain_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xe;
}

void __thiscall lifetap_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x14;
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall summons_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x17;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall hypnotize_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xf;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall iceshield2_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x16;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall iceshield3_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x16;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall pestilence_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x19;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_PESTILENCE_DAMAGE);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall cure_disease_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1a;
}

void __thiscall petrify_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x18;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_pain_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1b;
}

void __thiscall raise_dead_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x34;
}

void __thiscall death_grasp_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall inflexibility_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall weaken_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall dark_banishing_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x20;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_slowness_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x21;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_inflexibility_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x22;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_weaken_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x23;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_plague_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x24;
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall remediless_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x25;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_healing_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x26;
}

void __thiscall sentinel_healing_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x27;
}

void __thiscall charm_animal_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x45;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall thorn_shield_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall quickness_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x28;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall area_quickness_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x29;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall flexibility_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall area_flexibility_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall strength_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall area_strength_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall guard_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x2f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall remove_curse_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x48;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall regenerate_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x30;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall hallow_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x31;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall fireshield2_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x32;
}

void __thiscall thorn_shield2_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x33;
}

void __thiscall forget_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x37;
}

void __thiscall self_illusion_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x68;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall retention_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x38;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall brilliance_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x39;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall sacrifice_mana_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3a;
}

void __thiscall manatap_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3b;
}

void __thiscall manadrain_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3c;
}

void __thiscall shock_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x42;
}

void __thiscall disrupt_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3f;
}

void __thiscall confuse_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall rain_of_fire_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x41;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall blizzard_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x43;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall acid_cloud_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x40;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall stone_rain_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x44;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall amok_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x3d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall extinct_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x47;
}

void __thiscall detect_metal_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x64;
}

void __thiscall detect_magic_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x1e;
}

void __thiscall unkn_spell_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall invisibility_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x46;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall stone_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x10;
}

void __thiscall aura_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x49;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, EFFECT_EFFECT_INDEX);
}

void __thiscall suicide_death_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4d;
}

void __thiscall feign_death_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall dispel_white_aura_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x52;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall suffocation_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall inablility_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall slow_fighting_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall suicide_heal_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x51;
}

void __thiscall dominate_animal_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x54;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall dispel_black_aura_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x53;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall dexterity_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x50;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall endurance_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall fast_fighting_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x4f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
}

void __thiscall distract_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x63;
}

void __thiscall dominate_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall charm_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall befriend_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5d;
}

void __thiscall disenchant_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7c;
}

void __thiscall charisma_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x59;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall shockwave_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5c;
}

void __thiscall demoralization_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x75;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall enlightenment_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER2);
}

void __thiscall meditation_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x5b;
}

void __thiscall wave_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x55;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, EFFECT_EFFECT_INDEX);
}

void __thiscall melt_resistance_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x58;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall chill_resistance_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x57;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall rock_bullet_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x56;
	spellAPI.setXData(_this, spell_index, 0x45, 0);
}

void __thiscall conservation_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x65;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_CONSERVATION_SHIELD);
}

void __thiscall tower_arrow_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x60;
}

void __thiscall tower_healing_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x61;
}

void __thiscall tower_icestrike_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x62;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
}

void __thiscall lifetap_aura_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x66;
}

void __thiscall fireball2_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x67;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall ability_warcry_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x69;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_benefactions_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_patronize_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_endurance_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_berserk_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_boons_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_shelter_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x6f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_durability_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x70;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_trueshot_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x71;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_steelskin_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x72;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_salvo_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x73;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall spark_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x74;
}

void __thiscall tower_hypnotize_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x76;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall tower_pain_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x77;
}

void __thiscall tower_stone_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x78;
}

void __thiscall cloak_of_nor_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x79;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall unkn_spell2_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall freeze2_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall freeze3_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x7f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall lava_bullet_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x80;
}

void __thiscall tower_extinct_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x81;
}

void __thiscall manatap_aura_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x82;
}

void __thiscall firebane_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x83;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall black_essence_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x84;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall white_essence_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x86;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall chain_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa2;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall eternity_aura_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x91;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER2);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER3);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER4);
}

void __thiscall elemental_essence_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x88;
}

void __thiscall mental_essence_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8a;
}

void __thiscall black_almightness_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x85;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall white_almightness_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x87;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall elemental_almightness_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x89;
}

void __thiscall mental_almightness_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8b;
}

void __thiscall elemental_almightness2_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall elemental_essence2_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_DOUBLE_DAMAGE);
}

void __thiscall assistance_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8e;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall holy_touch_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x8f;
}

void __thiscall revenge_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x97;
}

void __thiscall area_roots_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x90;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall roots_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa3;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall reinforcement_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x99;
}

void __thiscall cannibalize_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x92;
}

void __thiscall torture_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x98;
}

void __thiscall dominate_undead_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x93;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall mutation_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9d;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER2);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER3);
	initializeSpellData(_this, spell_index, SPELL_STAT_MUL_MODIFIER4);
}

void __thiscall area_darkness_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x94;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_freeze_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x95;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall clay_feet_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9b;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall mirage_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa4;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall feedback_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa6;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_hypnotize_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9a;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall area_confuse_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9c;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall manashield_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa1;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall shift_mana_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x96;
}

void __thiscall ability_shift_life_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9e;
}

void __thiscall ability_riposte_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0x9f;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall ability_critical_hits_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa0;
	initializeSpellData(_this, spell_index, SPELL_TICK_COUNT_AUX);
}

void __thiscall belial_effect_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
	_this->active_spell_list[spell_index].spell_job = 0xa5;
}