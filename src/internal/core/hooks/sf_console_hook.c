/** 
 * @defgroup ConsoleHook Console Hook
 * @ingroup Hooks
 * Used for hooking into the Ingame Console
 */

#include "../sf_wrappers.h"
#include "sf_console_hook.h"

/**
 * @ingroup ConsoleHook
 */
static console_print_ptr s_console_print;

/**
 * @ingroup ConsoleHook
 */
void console_log(const char *message)
{
    if (s_console_print != NULL)
    {
        uint32_t _application = ASI::AddrOf(0x925C64);
        s_console_print(_application, message);
    }
    else
    {
        OutputDebugStringA(message);
    }
    // Appened to log file
    log_message("sfsf.log", message);
}

/**
 * @ingroup ConsoleHook
 */
void initialize_console_data_hooks()
{
    uint32_t CAppMain_ptr = ASI::AddrOf(0x9229A8);
    uint32_t CAppMenu_ptr = *(uint32_t *)(CAppMain_ptr + 0x4);
    uint32_t CMnuScrConsole_ptr = *(uint32_t *)(CAppMenu_ptr + 0x80);
    s_console_print = (console_print_ptr)ASI::AddrOf(0x1b58f0);
    g_create_sf_string = (SF_String_ctor_ptr)ASI::AddrOf(0x3837e0);
    g_destroy_sf_string = (SF_String_dtor_ptr)ASI::AddrOf(0x3839c0);
}