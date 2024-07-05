#include "../core/sf_hooks.h"
#include "../core/sf_wrappers.h"

#include "sf_spellrefresh_handlers.h"

int __thiscall first_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("first block refresh handler called");
    // This is the first block of spells in Ghidra, however we've made exceptions to this rule, by moving warcry, patronize, and endurence to a specific handler
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);

    if (hasSpell)
    {
        uint16_t spell_index_of_type = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
        if (spell_index_of_type)
        {
            spellAPI.removeDLLNode(_this, spell_line);
            spellAPI.onSpellRemove(_this, spell_line);
            spellAPI.setEffectDone(_this, spell_line, 0);
        }
    }

    return 1;
}

int __thiscall warcry_berserk_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    // Essentially, prevents warcry from being used if Berserk is active.
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;

    // We need the prelog, not sure if 100% required, but following vanilla logic is best.
    bool returnValue = first_block_refresh_handler(_this, spell_index);

    // Warcry Called
    if (spell_line == 0x94)
    {
        spell_line = 0x98;
    }
    else
    {
        // Hence if it's not 0x94, it's starting as 0x98, then we need to check for spell 0x94
        // This handler is designed only for warcry and beserk, using it elsewhere will result in undefined behavior
        spell_line = 0x94;
    }

    // we check if it has the incompatible spell on the target
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);
    if (!hasSpell)
        return returnValue;

    // If it does, we need to double check if the entity has the spell properly
    uint16_t spell_line_id = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
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
    // Essentially, prevents warcry from being used if Berserk is active.
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;

    // We need the prelog, not sure if 100% required, but following vanilla logic is best.
    bool returnValue = first_block_refresh_handler(_this, spell_index);

    if (spell_line == 0x96)
    {
        spell_line = 0x9a;
    }
    else
    {
        spell_line = 0x96;
    }

    // we check if it has the incompatible spell on the target
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);
    if (!hasSpell)
        return returnValue;

    // If it does, we need to double check if the entity has the spell properly
    uint16_t spell_line_id = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
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
    uint16_t target_entity_index = _this->active_spell_list[spell_index].target.entity_index;

    // We need the prelog, not sure if 100% required, but following vanilla logic is best.
    bool returnValue = first_block_refresh_handler(_this, spell_index);

    if (spell_line == 0x97)
    {
        spell_line = 0x9b;
    }
    else
    {
        spell_line = 0x97;
    }

    // we check if it has the incompatible spell on the target
    bool hasSpell = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, target_entity_index, spell_line);
    if (!hasSpell)
        return returnValue;

    // If it does, we need to double check if the entity has the spell properly
    uint16_t spell_line_id = toolboxAPI.getSpellIndexOfType(_this->SF_CGdFigureToolBox, target_entity_index, spell_line, spell_index);
    if (!spell_line_id)
        return returnValue;

    // In Ghidra, we'd break out of the main switch here, but in our handler we just directly call these functions
    spellAPI.removeDLLNode(_this, spell_line_id);
    spellAPI.onSpellRemove(_this, spell_line_id);
    spellAPI.setEffectDone(_this, spell_line_id, 0);
    return returnValue;
}

int __thiscall second_block_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("second block refresh handler called");
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;

    return 1;
}

int __thiscall case_da_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("case 0xda block refresh handler called");
    return 1;
}

int __thiscall case_17_refresh_handler(SF_CGdSpell *_this, uint16_t spell_index)
{
    log_info("case 0x17 block refresh handler called");
    int end_result = 1;

    uint16_t remove_id = spell_index;
    // A few checks here, this one can return 1 or 0
    uint16_t spell_line = _this->active_spell_list[spell_index].spell_line;
    uint16_t spell_id = _this->active_spell_list[spell_index].spell_id;

    SF_CGdResourceSpell spell_data;
    SF_CGdResourceSpell *ref = spellAPI.getResourceSpellData(_this->SF_CGdResource, &spell_data, spell_id);

    uint16_t spell_line_id = ref->spell_line_id;
    uint16_t entity_index = _this->active_spell_list[spell_index].target.entity_index;

    bool does_figure_have_spell_on_it = toolboxAPI.hasSpellOnIt(_this->SF_CGdFigureToolBox, entity_index, spell_line_id);

    if (does_figure_have_spell_on_it == 0)
    {
        end_result = 1;
    }
    else
    {
        spellAPI.removeDLLNode(_this, remove_id);
        spellAPI.onSpellRemove(_this, remove_id);
        spellAPI.setEffectDone(_this, remove_id, 0);
    }

    return end_result;
}