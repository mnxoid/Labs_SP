#include "util.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include <string>
#include <io.h>

#define BUF_SIZE 256
TCHAR szMsg[] = TEXT("Message from first process.");

int Proc1()
{
	HANDLE hMapFile;
	LPCTSTR pBuf;
	cout << szName << endl;
	cout << "Enter file name or 0 : ";
	string str;
	cin >> str;
	myGetch();
	FILE* f;
	if (str != "0") f = fopen(str.c_str(), "w"); else f = NULL;
	hMapFile = CreateFileMapping(
		f ? (HANDLE)_get_osfhandle(_fileno(f)): INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		f ? GENERIC_WRITE : PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName);                 // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			GetLastError());
		myGetch();
		return 1;
	}
	pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);
		myGetch();
		return 1;
	}


	CopyMemory((PVOID)pBuf, szMsg, (_tcslen(szMsg) * sizeof(TCHAR)));
	myGetch();

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}
