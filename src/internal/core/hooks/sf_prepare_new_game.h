#ifndef SF_PREPGAME_HOOK_H
#define SF_PREPGAME_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

typedef void (__thiscall *preloadUpdateKit2_ptr)(SF_GameInfo *_this, uint32_t unknown, uint8_t kit);              /* 0x9fad30 */
typedef void (__thiscall *preloadUpdateKit_ptr)(SF_GameInfo *_this, uint8_t unknown, uint8_t kit);              /* 0x9fad30 */
typedef uint32_t (__thiscall *preloadGetResultCode_ptr)(void *preload);              /* 0x9fad30 */
typedef void (__thiscall *gameInfoReset_ptr)(SF_GameInfo *gi, uint32_t p1);          /* 0x5a1830 */
typedef GdAvatarData *(__thiscall *preloadGetAvatar_ptr)(void *preload, void *out);  /* 0x9fae00 */
typedef void (__thiscall *gameInfoSetAvatar_ptr)(SF_GameInfo *gi, void *avatar,
                                                 uint16_t first_elem_u16);           /* 0x5a1440 */
typedef uint8_t (__thiscall *gameInfoGetU8_ptr)(SF_GameInfo *gi);                    /* kit 0x575f00 */
typedef uint8_t (__thiscall *gameInfoFigureAbilities_ptr)(SF_GameInfo *gi, uint32_t param_2);     /* skill 0x59e7a0 / subskill 0x59e7c0 */

typedef uint32_t (__thiscall *preloadGetU32_ptr)(void *preload);                     /* sotp side 0xa0a3d0 / campaign 0x9faa10 / */
typedef uint8_t (__thiscall *preloadGetU8_ptr)(void *preload);                          /* skiptut 0xa09780 */
typedef void (__thiscall *giSetBoolU8_ptr)(SF_GameInfo *gi, uint32_t b, uint8_t i);     /* 0x576760 / 0x5a1b10 */
typedef void (__thiscall *giSetU32_ptr)(SF_GameInfo *gi, uint32_t v);                /* mode 0x5766f0 / mode2 0x576780 / flags 0x576740,0x576750 */
typedef void (__thiscall *giSetStr_ptr)(SF_GameInfo *gi, SF_String *s);              /* savepath 0x576320 / savename 0x576700  */
typedef SF_String *(__thiscall *giSetTemplateName_ptr)(SF_GameInfo *gi, SF_String *s); // 0x576790 setTemplateName
typedef void (__thiscall *giVoid_ptr)(SF_GameInfo *gi);                              /* 0x576480 */
typedef void (__thiscall *giStarterKitReset_ptr)(SF_GameInfo *gi, uint32_t skill, uint32_t subskill);     /* 0x5763c0 needs double check*/
typedef SF_String *(__thiscall *preloadGetStr_ptr)(void *preload, SF_String *out);   /* 0xa09520 slot / 0xa09600 avatar / 0xa0a3e0 fg map / 0xa0a410 fg tmpl */
typedef void (__thiscall *appMenuReadSave_ptr)(CAppMenu *m, SF_String *s);         /* 0x586520 */
/* custom splash hints? */
typedef CMnuScreen *(__thiscall *getPregameScreen_ptr)(CMnuScreen *_this);                           /* 0x90f930 */
typedef SF_String *(__thiscall *getScreenName_ptr)(CMnuScreen *_this);                                 /* 0x910cf0 */
typedef uint8_t (__thiscall *screenDeleteControl_ptr)(CMnuScreen *screen, SF_String *ctl);           /* 0x908560 */
typedef void (__thiscall *screenSetActive_ptr)(CMnuScreen *screen, CMnuScreen *v);                   /* 0x912180 */
typedef void *(__thiscall   *cfgCtor_ptr)(CUtlConfigFile *cfg, const char *p);                   /* 0x789050 */
typedef void (__thiscall    *cfgDtor_ptr)(CUtlConfigFile *cfg);                                   /* 0x787e20 */
typedef void (__thiscall *cfgSetString_ptr)(CUtlConfigFile *cfg, char *section, char *key,
                                            SF_String *value, SF_String *dflt);      /* 007897b0 */

typedef void (__thiscall *CreateMnuHintExt_ptr)(CAppMenu *_this);
typedef void (__thiscall *CreateMenuPreMulti_ptr)(CAppMenu *_this, uint32_t param_1,uint32_t param_2,uint32_t param_3,
                                                  uint32_t param_4, uint32_t param_5, uint32_t param_6,
                                                  uint16_t param_7, SF_String *param_8,SF_String *param_9); /* 593980 */

typedef void (__thiscall *InitUnknownAvatar_0x54_ptr)(void *_this);
typedef uint8_t (__thiscall *updatePreloadUnknown_0x54_ptr)(CUiMenuPreLoad *_this, void *param1);
typedef GdAvatarInternal *(__thiscall *InitAvatarInternal_ptr)(void *buffer);
typedef uint32_t (__thiscall *importFromSave_ptr)(CAppMenu *_this,void *param_1, GdAvatarInternal *param_2);
#endif // SF_PREPGAME_HOOK_H
