#include "../sf_wrappers.h"

#include "sf_console_hook.h"

console_print_ptr console_print;

void console_log(const char *message)
{
    if (console_print != NULL)
    {
        uint32_t _application = ASI::AddrOf(0x925C64);
        console_print(_application, message);
    }
    else
    {
        OutputDebugStringA(message);
    }
    // Appened to log file
    log_message("sfsf.log", message);
}

void initialize_console_data_hooks()
{
    uint32_t CAppMain_ptr = ASI::AddrOf(0x9229A8);
    uint32_t CAppMenu_ptr = *(uint32_t *)(CAppMain_ptr + 0x4);
    uint32_t CMnuScrConsole_ptr = *(uint32_t *)(CAppMenu_ptr + 0x80);
    console_print = (console_print_ptr)ASI::AddrOf(0x1b58f0);
    SF_String_ctor = (SF_String_ctor_ptr)ASI::AddrOf(0x3837e0);
    SF_String_dtor = (SF_String_dtor_ptr)ASI::AddrOf(0x3839c0);
}