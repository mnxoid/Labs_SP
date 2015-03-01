#include <windows.h>
#include <stdio.h>
#include <sddl.h>
#define _WIN32_WINNT 0x0500
#pragma comment(lib, "advapi32.lib")
void CreateSelfProcess(LPCTSTR mname, LPTSTR pname)//,LPSECURITY_ATTRIBUTES sa)
{
	STARTUPINFO* si1 = new STARTUPINFO;
	PROCESS_INFORMATION* pi1 = new PROCESS_INFORMATION;

	ZeroMemory(si1, sizeof(*si1));
	si1->cb = sizeof(*si1);
	ZeroMemory(pi1, sizeof(*pi1));
	CreateProcess(mname, pname, NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, si1, pi1);
}

int GetKeyData(HKEY hRootKey, char *subKey, char *value, LPBYTE data, DWORD cbData)
{
	HKEY hKey;
	if (RegOpenKeyEx(hRootKey, subKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		return 0;

	if (RegQueryValueEx(hKey, value, NULL, NULL, data, &cbData) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 0;
	}

	RegCloseKey(hKey);
	return 1;
}

int SetKeyData(HKEY hRootKey, char *subKey, DWORD dwType, char *value, LPBYTE data, DWORD cbData)
{
	HKEY hKey;
	if (RegCreateKey(hRootKey, subKey, &hKey) != ERROR_SUCCESS)
		return 0;

	if (RegSetValueEx(hKey, value, 0, dwType, data, cbData) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 0;
	}

	RegCloseKey(hKey);
	return 1;
}
void myGetch()
{
	while (getchar() != '\n');
}
// CreateMyDACL.
//    Create a security descriptor that contains the DACL 
//    you want.
//    This function uses SDDL to make Deny and Allow ACEs.
//
// Parameter:
//    SECURITY_ATTRIBUTES * pSA
//    Pointer to a SECURITY_ATTRIBUTES structure. It is your
//    responsibility to properly initialize the 
//    structure and to free the structure's 
//    lpSecurityDescriptor member when you have
//    finished using it. To free the structure's 
//    lpSecurityDescriptor member, call the 
//    LocalFree function.
// 
// Return value:
//    FALSE if the address to the structure is NULL. 
//    Otherwise, this function returns the value from the
//    ConvertStringSecurityDescriptorToSecurityDescriptor 
//    function.
BOOL CreateMyDACL(SECURITY_ATTRIBUTES * pSA)
{
	// Define the SDDL for the DACL. This example sets 
	// the following access:
	//     Built-in guests are denied all access.
	//     Anonymous logon is denied all access.
	//     Authenticated users are allowed 
	//     read/write/execute access.
	//     Administrators are allowed full control.
	// Modify these values as needed to generate the proper
	// DACL for your application. 
	TCHAR * szSD = TEXT("D:")       // Discretionary ACL
		TEXT("(D;OICI;GA;;;BG)")     // Deny access to 
		// built-in guests
		TEXT("(D;OICI;GA;;;AN)")     // Deny access to 
		// anonymous logon
		TEXT("(A;OICI;GRGWGX;;;AU)") // Allow 
		// read/write/execute 
		// to authenticated 
		// users
		TEXT("(A;OICI;GA;;;BA)");    // Allow full control 
	// to administrators

	if (NULL == pSA)
		return FALSE;

	return ConvertStringSecurityDescriptorToSecurityDescriptor(
		szSD,
		SDDL_REVISION_1,
		&(pSA->lpSecurityDescriptor),
		NULL);
}