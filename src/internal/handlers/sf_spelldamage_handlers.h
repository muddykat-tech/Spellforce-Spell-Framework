#ifndef SF_SPELLDAMAGE_HANDLER_H
#define SF_SPELLDAMAGE_HANDLER_H

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

uint16_t __thiscall mana_shield_dmg_handler(SF_CGdFigureToolbox *_this,
                                            uint16_t source, uint16_t target,
                                            uint16_t current_damage,
                                            uint16_t is_spell_damage,
                                            uint32_t is_ranged_damage,
                                            uint16_t spell_id);

uint16_t __thiscall death_grasp_dmg_handler(SF_CGdFigureToolbox *_this,
                                            uint16_t source, uint16_t target,
                                            uint16_t current_damage,
                                            uint16_t is_spell_damage,
                                            uint32_t is_ranged_damage,
                                            uint16_t spell_id);

uint16_t __thiscall feedback_dmg_handler(SF_CGdFigureToolbox *_this,
                                         uint16_t source, uint16_t target,
                                         uint16_t current_damage,
                                         uint16_t is_spell_damage,
                                         uint32_t is_ranged_damage,
                                         uint16_t spell_id);

uint16_t __thiscall feign_death_dmg_handler(SF_CGdFigureToolbox *_this,
                                            uint16_t source, uint16_t target,
                                            uint16_t current_damage,
                                            uint16_t is_spell_damage,
                                            uint32_t is_ranged_damage,
                                            uint16_t spell_id);

uint16_t __thiscall conservation_dmg_handler(SF_CGdFigureToolbox *_this,
                                             uint16_t source, uint16_t target,
                                             uint16_t current_damage,
                                             uint16_t is_spell_damage,
                                             uint32_t is_ranged_damage,
                                             uint16_t spell_id);

uint16_t __thiscall dmg_shield_dmg_handler(SF_CGdFigureToolbox *_this,
                                           uint16_t source, uint16_t target,
                                           uint16_t current_damage,
                                           uint16_t is_spell_damage,
                                           uint32_t is_ranged_damage,
                                           uint16_t spell_id);

uint16_t __thiscall invulnerability_dmg_handler(SF_CGdFigureToolbox *_this,
                                                uint16_t source,
                                                uint16_t target,
                                                uint16_t current_damage,
                                                uint16_t is_spell_damage,
                                                uint32_t is_ranged_damage,
                                                uint16_t spell_id);

uint16_t __thiscall durability_dmg_handler(SF_CGdFigureToolbox *_this,
                                           uint16_t source, uint16_t target,
                                           uint16_t current_damage,
                                           uint16_t is_spell_damage,
                                           uint32_t is_ranged_damage,
                                           uint16_t spell_id);

uint16_t __thiscall steelskin_dmg_handler(SF_CGdFigureToolbox *_this,
                                          uint16_t source, uint16_t target,
                                          uint16_t current_damage,
                                          uint16_t is_spell_damage,
                                          uint32_t is_ranged_damage,
                                          uint16_t spell_id);

uint16_t __thiscall hypnotize_dmg_handler(SF_CGdFigureToolbox *_this,
                                          uint16_t source, uint16_t target,
                                          uint16_t current_damage,
                                          uint16_t is_spell_damage,
                                          uint32_t is_ranged_damage,
                                          uint16_t spell_id);

#endif
