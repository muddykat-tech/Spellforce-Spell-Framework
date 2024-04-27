#include <windows.h>
#include <stdio.h> 
#include <string.h>
#include "test_loader.h"

int add(int a, int b)
{
	return a+b;
}

void listDllFiles(const char *folderPath) {
    WIN32_FIND_DATA findData;
    HANDLE hFind;
    char searchPath[MAX_PATH];
    sprintf(searchPath, "%s\\sfsf\\*.dll", folderPath);

    hFind = FindFirstFile(searchPath, &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            printf("%s\n", findData.cFileName);
        } while (FindNextFile(hFind, &findData) != 0);
        FindClose(hFind);
    } else {
        printf("No DLL files found in the specified directory.\n", folderPath);
	}
}

int main() {
    char folderPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, folderPath);

    listDllFiles(folderPath);

	FrameworkAPI FAPI;
	FAPI.add = &add;
    HMODULE hinstLib = LoadLibrary("sfsf\\test_library.dll");
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
