#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"

#include "../registry/sf_mod_registry.h"

#include "sf_spellrefresh_handlers.h"

// default case for spells with duration refresh
int __thiscall first_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("first block refresh handler called");
    // This is the first block of spells in Ghidra, however we've made exceptions to this rule, by moving warcry, patronize, and endurence to a specific handler
    // moving stuff covered bu "onSpellRemove" to specific handlers ~UnSchtalch
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);

    if (hasSpell)
    {
        uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
        if (spell_index_of_type)
        {
            spellAPI.removeDLLNode(_this, spell_line);
            spellAPI.setEffectDone(_this, spell_line, 0);
        }
    }

    return 1;
}
// case 0x04
int __thiscall slowness_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);

    if (hasSpell)
    {
        uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
        if (spell_index_of_type)
        {
            spellAPI.removeDLLNode(_this, spell_index_of_type);
            SF_CGdResourceSpell spell_data;
            spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index_of_type].spell_id);
            figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WALK_SPEED, target_entity_index, spell_data.params[0]);
            spellAPI.setEffectDone(_this, spell_index_of_type, 0);
        }
    }

    return 1;
}

// case 0x10
int __thiscall decay_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);

    if (hasSpell)
    {
        uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
        if (spell_index_of_type)
        {
            spellAPI.removeDLLNode(_this, spell_index_of_type);
            SF_CGdResourceSpell spell_data;
            spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index_of_type].spell_id);
            figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, ARMOR, target_entity_index, spell_data.params[2]);
            spellAPI.setEffectDone(_this, spell_index_of_type, 0);
        }
    }
    return 1;
}

// case 0x22
int __thiscall weaken_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);

    if (hasSpell)
    {
        uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
        if (spell_index_of_type)
        {
            spellAPI.removeDLLNode(_this, spell_index_of_type);
            SF_CGdResourceSpell spell_data;
            spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index_of_type].spell_id);
            figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, AGILITY, target_entity_index, spell_data.params[0]);
            spellAPI.setEffectDone(_this, spell_index_of_type, 0);
        }
    }
    return 1;
}

// case 0x23
int __thiscall inflexibility_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);

    if (hasSpell)
    {
        uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
        if (spell_index_of_type)
        {
            spellAPI.removeDLLNode(_this, spell_index_of_type);
            SF_CGdResourceSpell spell_data;
            spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, _this->active_spell_list[spell_index_of_type].spell_id);
            int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index_of_type, SPELL_STAT_MUL_MODIFIER));

            figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WALK_SPEED, target_entity_index, bonus);
            spellAPI.setEffectDone(_this, spell_index_of_type, 0);
        }
    }
    return 1;
}

// case 0x30
int __thiscall quickness_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);

    if (hasSpell)
    {
        uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
        if (spell_index_of_type)
        {
            spellAPI.removeDLLNode(_this, spell_index_of_type);
            int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index_of_type, SPELL_STAT_MUL_MODIFIER));
            figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, WALK_SPEED, target_entity_index, bonus);
            spellAPI.setEffectDone(_this, spell_index_of_type, 0);
        }
    }
    return 1;
}

// case 0x32
int __thiscall flexibility_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);

    if (hasSpell)
    {
        uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
        if (spell_index_of_type)
        {
            spellAPI.removeDLLNode(_this, spell_index_of_type);
            int8_t bonus = (int8_t)(-spellAPI.getXData(_this, spell_index_of_type, SPELL_STAT_MUL_MODIFIER));
            figureAPI.addBonusMultToStatistic(_this->SF_CGdFigure, AGILITY, target_entity_index, bonus);
            spellAPI.setEffectDone(_this, spell_index_of_type, 0);
        }
    }
    return 1;
}

int __thiscall warcry_berserk_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{

    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t spell_id = _this->active_spell_list[spell_index].spell_id;

    SF_CGdResourceSpell spell_data;
    SF_CGdResourceSpell *ref = spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);

    uint16_t spell_line_id = ref->spell_line_id;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;

    // We need the prelog, not sure if 100% required, but following vanilla logic is best.
    bool returnValue = first_block_refresh_handler(_this, spell_index);

    if (spell_line_id == 0x94)
    {
        spell_line_id = 0x98;
    }
    else
    {
        spell_line_id = 0x94;
    }

    // we check if it has the incompatible spell on the target
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line_id);
    if (!hasSpell)
        return returnValue;

    // If it does, we need to double check if the entity has the spell properly
    spell_line_id = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line_id, spell_index);

    if (!spell_line_id)
        return returnValue;

    // In Ghidra, we'd break out of the main switch here, but in our handler we just directly call these functions
    spellAPI.removeDLLNode(_this, spell_line_id);
    spellAPI.onSpellRemove(_this, spell_line_id);
    spellAPI.setEffectDone(_this, spell_line_id, 0);

    return returnValue;
}

int __thiscall patronize_shelter_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t spell_id = _this->active_spell_list[spell_index].spell_id;

    SF_CGdResourceSpell spell_data;
    SF_CGdResourceSpell *ref = spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);

    uint16_t spell_line_id = ref->spell_line_id;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;

    // We need the prelog, not sure if 100% required, but following vanilla logic is best.
    bool returnValue = first_block_refresh_handler(_this, spell_index);

    if (spell_line_id == 0x96)
    {
        spell_line_id = 0x9a;
    }
    else
    {
        spell_line_id = 0x96;
    }

    // we check if it has the incompatible spell on the target
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line_id);
    if (!hasSpell)
        return returnValue;

    // If it does, we need to double check if the entity has the spell properly
    spell_line_id = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line_id, spell_index);

    if (!spell_line_id)
        return returnValue;

    // In Ghidra, we'd break out of the main switch here, but in our handler we just directly call these functions
    spellAPI.removeDLLNode(_this, spell_line_id);
    spellAPI.onSpellRemove(_this, spell_line_id);
    spellAPI.setEffectDone(_this, spell_line_id, 0);

    return returnValue;
}

int __thiscall endurence_durability_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // Essentially, prevents warcry from being used if Berserk is active.
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t spell_id = _this->active_spell_list[spell_index].spell_id;

    SF_CGdResourceSpell spell_data;
    SF_CGdResourceSpell *ref = spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);

    uint16_t spell_line_id = ref->spell_line_id;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;

    // We need the prelog, not sure if 100% required, but following vanilla logic is best.
    bool returnValue = first_block_refresh_handler(_this, spell_index);

    if (spell_line_id == 0x97)
    {
        spell_line_id = 0x9b;
    }
    else
    {
        spell_line_id = 0x97;
    }

    // we check if it has the incompatible spell on the target
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line_id);
    if (!hasSpell)
        return returnValue;

    // If it does, we need to double check if the entity has the spell properly
    spell_line_id = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line_id, spell_index);

    if (!spell_line_id)
        return returnValue;

    // In Ghidra, we'd break out of the main switch here, but in our handler we just directly call these functions
    spellAPI.removeDLLNode(_this, spell_line_id);
    spellAPI.onSpellRemove(_this, spell_line_id);
    spellAPI.setEffectDone(_this, spell_line_id, 0);
    return returnValue;
}

// case 17
int __thiscall pestilence_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    uint16_t remove_id = spell_index;
    uint16_t spell_id = _this->active_spell_list[spell_index].spell_id;

    SF_CGdResourceSpell spell_data;
    SF_CGdResourceSpell *ref = spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);

    uint16_t spell_line_id = ref->spell_line_id;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;

    bool does_figure_have_spell_on_it = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line_id);

    if (does_figure_have_spell_on_it == 0)
        return 1;

    // Ghidra LAB GOTO LABEL
    spell_line_id = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line_id, spell_index);
    if (!spell_line_id)
        return 1;

    return 0;
}

int __thiscall domination_spell_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("Domination Spell Refresh Handler Called");
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t spell_id = _this->active_spell_list[spell_index].spell_id;

    SF_CGdResourceSpell spell_data;
    SF_CGdResourceSpell *ref = spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);

    uint16_t spell_line_id = ref->spell_line_id;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;
    // This is quite literially, just so domination spells do not stack
    // if any of these spells are on the target, we return 0

    // we need to get the spell ids of all spells with the internal tag 'DOMINATION'
    std::list<uint16_t> domination_spell_ids;
    for (SFSpell *spell : internal_spell_list)
    {
        if (spell->spell_tag == SpellTag::DOMINATION_SPELL)
        {
            domination_spell_ids.push_back(spell->spell_id);
        }
    }

    log_info("Check 1");
    // Now we check if the target has any of these spells
    bool hasDominationSpell = false;
    for (uint16_t spell_id : domination_spell_ids)
    {
        if (toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_id) == 0)
        {
            if (toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_id, spell_index) != 0)
            {
                hasDominationSpell = true;
            }
        }
    }

    return hasDominationSpell ? 0 : 1;
}

int __thiscall case_da_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("case 0xda block refresh handler called");
    return 1;
}
