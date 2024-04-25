#include <windows.h>

typedef int (*test_called_function)(int, int);

struct _framework_api
{
	test_called_function add;
};

typedef struct _framework_api FrameworkAPI;
typedef struct _framework_api *pFrameworkAPI;

pFrameworkAPI pFAPI;

__declspec(dllexport) void __stdcall InitModule(void * pInterface)
{
	pFAPI = (pFrameworkAPI) pInterface;
}

__declspec(dllexport) int __stdcall ExportedFunction()
{
	return pFAPI->add(2, 2);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  switch (fdwReason)
  {
    case DLL_PROCESS_ATTACH:
      /* Code path executed when DLL is loaded into a process's address space. */
      break;

    case DLL_THREAD_ATTACH:
      /* Code path executed when a new thread is created within the process. */
      break;

    case DLL_THREAD_DETACH:
      /* Code path executed when a thread within the process has exited *cleanly*. */
      break;

    case DLL_PROCESS_DETACH:
      /* Code path executed when DLL is unloaded from a process's address space. */
      break;
  }

  return TRUE;
}