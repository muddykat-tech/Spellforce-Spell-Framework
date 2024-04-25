#include <windows.h>
#include <stdio.h> 

int add(int a, int b)
{
	return a+b;
}

typedef int (*test_called_function)(int, int);
typedef int (*test_return)();
typedef void (*test_caller_function_p)(void *);
struct _framework_api
{
	test_called_function add;
};
typedef struct _framework_api FrameworkAPI;
typedef struct _framework_api *pFrameworkAPI;



int main()
{
	FrameworkAPI FAPI;
	FAPI.add = &add;
    HMODULE hinstLib = LoadLibrary("test_library.dll");
	if(hinstLib)
	{
		test_caller_function_p tf = (test_caller_function_p) GetProcAddress(hinstLib, "InitModule");
		test_return tr = (test_return)GetProcAddress(hinstLib, "ExportedFunction");
		if (tf)
		{
			tf(&FAPI);
		}
		else
		{
			printf("Failed to get address of InitModule %d\n", GetLastError());	
		}
		if (tr)
		{
			printf("result: %d\n", tr());
		}
		else
		{
			printf("Failed to get address of ExportedFunction\n");	
		}
		FreeLibrary(hinstLib); 
	}
	return 0;
}