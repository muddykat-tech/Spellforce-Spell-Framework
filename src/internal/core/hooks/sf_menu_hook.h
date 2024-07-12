#ifndef SF_MENU_HOOK_H
#define SF_MENU_HOOK_H
#include "../../../api/sfsf.h"
#include "../../../asi/sf_asi.h"

extern original_menu_func_ptr original_menu_func;

void __thiscall sf_menu_hook(uint32_t _CAppMenu);

#endif