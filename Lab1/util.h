#pragma once
#include <windows.h>
#define MAX_SEM_COUNT 5
#define THREADCOUNT 12
extern void CreateSelfProcess(LPCTSTR mname, LPTSTR pname);// , LPSECURITY_ATTRIBUTES sa);
extern int GetKeyData(HKEY hRootKey, char *subKey, char *value, LPBYTE data, DWORD cbData);
extern int SetKeyData(HKEY hRootKey, char *subKey, DWORD dwType, char *value, LPBYTE data, DWORD cbData);
extern void myGetch();
extern BOOL CreateMyDACL(SECURITY_ATTRIBUTES * pSA);
extern int Proc1();
extern int Proc2();
extern int Proc3();
extern int Proc4();
extern int Proc5();