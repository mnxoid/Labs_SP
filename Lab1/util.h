#pragma once
#include <windows.h>
#define MAX_SEM_COUNT 5
#define THREADCOUNT 12
extern void CreateSelfProcess(LPCTSTR mname, LPTSTR pname);
extern int Proc1();
extern int Proc2();
extern int Proc3();