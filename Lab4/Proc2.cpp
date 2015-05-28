#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include "util.h"

int Proc2()
{
	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szName);               // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		myGetch();
		return 1;
	}

	pBuf = (LPTSTR)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
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

	MessageBox(NULL, pBuf, TEXT("Process2"), MB_OK);
	myGetch();
	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}