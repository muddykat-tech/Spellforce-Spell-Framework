#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_loader.h"

// Function definition
int add(int a, int b) {
    return a + b;
}

// Function to search for files in a directory with a given suffix
void searchFiles(const char* directory, const char* suffix, char fileList[][MAX_PATH], int* numFiles) {
    HANDLE hFind;
    WIN32_FIND_DATA findFileData;
    char searchPath[MAX_PATH];

    // Create the search path
    sprintf(searchPath, "%s\\bin\\sfsf\\*%s", directory, suffix);

    // Find the first file in the directory
    hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: No files found.\n");
        return;
    }

    // Add the first file to the list
    strcpy(fileList[*numFiles], findFileData.cFileName);
    (*numFiles)++;

    // Find the rest of the files in the directory
    while (FindNextFile(hFind, &findFileData) != 0) {
        strcpy(fileList[*numFiles], findFileData.cFileName);
        (*numFiles)++;
    }

    // Close the search handle
    FindClose(hFind);
}

// Function to run the loaded library
void run_lib(const char* lib_name) {
    FrameworkAPI FAPI;
    FAPI.add = add;
    char lib_path[MAX_PATH];

    // Create the library path
    sprintf(lib_path, "sfsf\\%s", lib_name);

    // Load the library
    HMODULE hinstLib = LoadLibrary(lib_path);
    if (hinstLib) {
        test_caller_function_p tf = (test_caller_function_p)GetProcAddress(hinstLib, "InitModule");
        test_return tr = (test_return)GetProcAddress(hinstLib, "ExportedFunction");
        if (tf) {
            tf(&FAPI);
        }
        else {
            printf("Failed to get address of InitModule %lu\n", GetLastError());
        }
        if (tr) {
            printf("result: %d\n", tr());
        }
        else {
            printf("Failed to get address of ExportedFunction\n");
        }
        FreeLibrary(hinstLib);
    }
}

int main() {
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    char directory[MAX_PATH];
    strcpy(directory, currentDir);

    char fileList[100][MAX_PATH]; // Assuming maximum 100 files atm config option?
    int numFiles = 0;

    searchFiles(directory, ".dll", fileList, &numFiles);

    printf("Files with suffix '.dll' in directory '%s':\n", directory);
    for (int i = 0; i < numFiles; i++) {
        run_lib(fileList[i]);
    }

    return 0;
}

