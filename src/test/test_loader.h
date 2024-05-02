#pragma once

typedef int (*test_called_function)(int, int);
typedef int (*test_return)();
typedef void (*test_caller_function_p)(void *);
struct _framework_api
{
	test_called_function add;
};
typedef struct _framework_api FrameworkAPI;
typedef struct _framework_api *pFrameworkAPI;
