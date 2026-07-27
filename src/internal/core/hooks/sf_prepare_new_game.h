#ifndef SF_PREPGAME_HOOK_H
#define SF_PREPGAME_HOOK_H

#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

typedef uint32_t (__thiscall *preloadGetResultCode_ptr)(void *preload);              /* 0x9fad30 */
typedef void     (__thiscall *gameInfoReset_ptr)(SF_GameInfo *gi, uint32_t p1);      /* 0x5a1830 */
typedef GdAvatarData *(__thiscall *preloadGetAvatar_ptr)(void *preload, void *out);  /* 0x9fae00 */
typedef void     (__thiscall *gameInfoSetAvatar_ptr)(SF_GameInfo *gi, void *avatar,
                                                     uint16_t first_elem_u16);       /* 0x5a1440 */
typedef uint8_t  (__thiscall *gameInfoGetU8_ptr)(SF_GameInfo *gi);                   /* skill 0x59e7a0 / subskill 0x59e7c0 / kit 0x575f00 */
typedef uint32_t (__thiscall *preloadGetU32_ptr)(void *preload);                     /* sotp side 0xa0a3d0 / campaign 0x9faa10 / skiptut 0xa09780 */
typedef void     (__thiscall *giSetBoolU8_ptr)(SF_GameInfo *gi, uint32_t b, uint8_t i); /* 0x576760 / 0x5a1b10 */
typedef void     (__thiscall *giSetU32_ptr)(SF_GameInfo *gi, uint32_t v);            /* mode 0x5766f0 / mode2 0x576780 / flags 0x576740,0x576750 */
typedef void     (__thiscall *giSetStr_ptr)(SF_GameInfo *gi, SF_String *s);          /* savepath 0x576320 / savename 0x576700  */
typedef SF_String* (__thiscall *giSetTemplateName_ptr)(SF_GameInfo *gi, SF_String *s); // 0x576790 setTemplateName
typedef void     (__thiscall *giVoid_ptr)(SF_GameInfo *gi);                          /* 0x576480 */
typedef void     (__fastcall *giStarterKitReset_ptr)(SF_GameInfo *gi, uint32_t edx_unused,
                                                     uint32_t skill, uint32_t subskill); /* 0x5763c0 needs double check*/
typedef SF_String *(__thiscall *preloadGetStr_ptr)(void *preload, SF_String *out);   /* 0xa09520 slot / 0xa09600 avatar / 0xa0a3e0 fg map / 0xa0a410 fg tmpl */
typedef void     (__thiscall *appMenuDeleteSave_ptr)(CAppMenu *m, SF_String *s);     /* 0x586520 */
/* custom splash hints? */
typedef uint32_t      (__thiscall *screenGetHint_ptr)(void *screen);                      /* 0x90f930 */
typedef uint32_t      (__thiscall *hintResolve_ptr)(int hint);                            /* 0x910cf0 */
typedef uint8_t     (__thiscall *screenDeleteControl_ptr)(void *screen, int ctl);       /* 0x908560 */
typedef void     (__thiscall *screenSetHint_ptr)(void *screen, int v);               /* 0x912180 */
typedef void*     (__thiscall   *cfgCtor_ptr)(void *cfg, const char *p);                /* 0x789050 */
typedef void     (__thiscall    *cfgDtor_ptr)(void *cfg);                               /* 0x787e20 */
typedef void     (__thiscall *cfgSetString_ptr)(void *cfg, char *section, char *key,
                                                SF_String *value, SF_String *dflt);

typedef void (__thiscall *CreateMnuHintExt_ptr)(CAppMenu *_this);


#endif // SF_PREPGAME_HOOK_H
