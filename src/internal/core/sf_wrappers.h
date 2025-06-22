#ifndef UTILITY_H
#define UTILITY_H

#include "../../api/sfsf.h"
#include "../../asi/sf_asi.h"

typedef SF_String *(__thiscall *SF_String_ctor_ptr)(SF_String *, const char *);
typedef void (__thiscall *SF_String_dtor_ptr)(SF_String *);
typedef bool (__thiscall *has_spell_effect_ptr)(SF_CGdFigureToolbox *_this,
                                                uint16_t param_1,
                                                uint16_t param_2);
void log_message(const char *filename, const char *message);

void initialize_wrapper_data_hooks();

extern bool __thiscall hasAuraActive(SF_CGdFigureToolbox *_this, uint16_t figure_id);
extern bool __thiscall isActionMelee(SF_SGtFigureAction *_this);
extern bool __thiscall hasSpellTag(uint16_t spell_id, SpellTag tag);
extern void __thiscall setupFigureIterator(CGdFigureIterator *iterator, SF_CGdSpell *spell);
extern void __thiscall disposeFigureIterator(CGdFigureIterator *iterator);

extern void __thiscall spellEffectCallback(SF_CGdSpell *_this,
                                           uint16_t source_index,
                                           uint16_t spell_index,
                                           bool (*condition)(SF_CGdSpell *_this,
                                                             uint16_t
                                                             spell_index,
                                                             uint16_t
                                                             walked_index),
                                           void (*callback)(SF_CGdSpell *_this,
                                                            uint16_t
                                                            source_index,
                                                            uint16_t
                                                            walked_index,
                                                            uint16_t spell_index));

extern void __thiscall addBonusMultToStatistic(SF_CGdFigure *figure,
                                               StatisticDataKey key,
                                               uint16_t target, int8_t value);
extern void __thiscall spellClearFigureFlag(SF_CGdSpell *spell,
                                            uint16_t spell_index,
                                            SpellFlagKey key);
extern uint16_t __thiscall sf_get_spell_id(SF_CGdSpell *_this,
                                           uint16_t spell_index);
extern uint32_t getDistance(SF_Coord *pointA, SF_Coord *pointB);
extern SFLog *setup_logger();

extern uint16_t __thiscall getPhysDamageReduction(SF_CGdFigureToolbox *_this, uint16_t source_index,
                                                  uint16_t target_index,
                                                  uint16_t action_id);


typedef void (__thiscall *vfunction_2_ptr)(void *_this, void *input);
typedef void (__thiscall *vfunction_ptr)(void *_this, char p1);
typedef void (__thiscall *vfunction12_ptr)(void *container, void *test,
                                           char p1);
typedef void (__thiscall *attach_string_ptr)(void *container, void *string);
typedef void (__thiscall *set_button_flag_ptr)(void *container, char flag);
typedef void (__thiscall *set_btn_index_ptr)(void *container, int index);
typedef void (__thiscall *create_button_ptr)(CMnuSmpButton *smpButton,
                                             float x_pos, float y_pos,
                                             float width, float height,
                                             SF_String *default_mesh,
                                             SF_String *unkn_mesh_string,
                                             SF_String *pressed_mesh,
                                             SF_String *disable_mesh);
typedef void (__thiscall *set_label_color_ptr)(CMnuLabel *_this, float r,
                                               float g, float b, char flag);
typedef void (__thiscall *set_btn_name_ptr)(void *button, SF_String *string);
typedef CMnuSmpButton *(__thiscall *initialize_smp_button_ptr)(
    CMnuSmpButton *btn);
typedef void (__thiscall *vfunction2_callback_attach_ptr)(void *, void *,void *,
                                                          void *);
typedef void (__thiscall *initialize_menu_container_ptr)(CMnuContainer *_this);
typedef void (__thiscall *setup_menu_container_data_ptr)(CMnuContainer *_this,
                                                         float x, float y,
                                                         float width,
                                                         float height,
                                                         SF_String *background,
                                                         SF_String *border);
typedef void (__thiscall *container_alpha_ptr)(CMnuContainer *_this,
                                               float alpha);
typedef void (__thiscall *CMnuBase_setname_ptr)(CMnuBase *_this,
                                                SF_String *name);

typedef uint16_t (__thiscall *get_phys_damage_reduction_ptr)(void *AutoClass34, uint16_t source_index,
                                                             uint16_t target_index,
                                                             uint16_t unkn);
extern FUN_0069eaf0_ptr FUN_0069eaf0;
extern fidfree_ptr fidFree;
extern SF_String_ctor_ptr g_create_sf_string;
extern SF_String_dtor_ptr g_destroy_sf_string;
extern has_spell_effect_ptr has_spell_effect;
extern create_button_ptr create_button_func;
extern attach_string_ptr vfunction_apply_string;
extern vfunction_ptr vfunction176;
extern vfunction_ptr vfunction25;
extern vfunction12_ptr vfunction12;
extern initialize_smp_button_ptr initialize_smp_button;
extern set_btn_name_ptr set_button_name;
extern initialize_menu_container_ptr initialize_menu_container;
extern set_label_color_ptr set_label_color;
extern container_alpha_ptr set_container_alpha;
extern setup_menu_container_data_ptr setup_menu_container_data;
extern vfunction_2_ptr set_font;
extern set_btn_index_ptr set_button_index;
extern set_button_flag_ptr set_menu_button_flag;
extern vfunction2_callback_attach_ptr attach_callback;
extern vfunction_ptr vfunction16_attach_callback;
extern get_phys_damage_reduction_ptr g_get_reduced_damage;


extern "C" void log_warning(const char *message);
extern "C" void log_info(const char *message);
extern "C" void log_error(const char *message);

#endif
