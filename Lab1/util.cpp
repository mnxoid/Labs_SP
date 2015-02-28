#include <windows.h>
void CreateSelfProcess(LPCTSTR mname, LPTSTR pname)
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