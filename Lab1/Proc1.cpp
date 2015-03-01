#include <iostream>
#include <windows.h>
#include "util.h"
using namespace std;
int Proc1()
{
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -100000000LL;
	HWND hwnd = GetActiveWindow();
	SECURITY_ATTRIBUTES  sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;

	// Call function to set the DACL. The DACL
	// is set in the SECURITY_ATTRIBUTES 
	// lpSecurityDescriptor member.
	if (!CreateMyDACL(&sa))
	{
		// Error encountered; generate message and exit.
		printf("Failed CreateMyDACL\n");
		exit(1);
	}
	HANDLE timerHandle = CreateWaitableTimerEx(NULL, "MnxoidTimer", 0, TIMER_ALL_ACCESS);
	if (timerHandle != NULL)
	{
		cout << "Timer created!" << endl;
	}
	HANDLE mutexHandle = CreateMutexEx(NULL, "MnxoidMutex", 0, MUTEX_ALL_ACCESS);
	if (mutexHandle != NULL)
	{
		cout << "Mutex created!" << endl;
	}
	HANDLE semaphoreHandle = CreateSemaphoreEx(NULL, MAX_SEM_COUNT, MAX_SEM_COUNT, "MnxoidSemaphore", 0, SEMAPHORE_ALL_ACCESS);
	if (semaphoreHandle!=NULL)
	{
		cout << "Semaphore created!" << endl;
	}
	if (!SetWaitableTimer(timerHandle, &liDueTime, 1000, NULL, NULL, 0))
	{
		cout << "SetWaitableTimer failed (" << GetLastError() << ")" << endl;
		return 0;
	}
	if (WaitForSingleObject(timerHandle, INFINITE) != WAIT_OBJECT_0)
		cout << "WaitForSingleObject failed (" << GetLastError() << ")" << endl;
	else cout << "Timer was signaled." << endl;

	myGetch();
	cout << endl << "Now it's time to create process 5" << endl;
	myGetch();
	LPTSTR lpFilename = (LPTSTR)calloc(1, 256);
	GetModuleFileName(NULL, lpFilename, 255);
	STARTUPINFO* si1 = new STARTUPINFO;
	PROCESS_INFORMATION* pi1 = new PROCESS_INFORMATION;
	ZeroMemory(si1, sizeof(*si1));
	si1->cb = sizeof(*si1);
	ZeroMemory(pi1, sizeof(*pi1));

	
	CreateProcess((LPCTSTR)lpFilename, "Process5",&sa,NULL,true,CREATE_NEW_CONSOLE,NULL,NULL,si1,pi1);
	if (!DuplicateHandle(GetCurrentProcess(), mutexHandle, pi1->hProcess, NULL, DUPLICATE_SAME_ACCESS, true, DUPLICATE_SAME_ACCESS))
	{
		cout << "DuplicateHandle failed (" << GetLastError() << ")" << endl;
	}
	DuplicateHandle(GetCurrentProcess(), semaphoreHandle, pi1->hProcess, NULL, DUPLICATE_SAME_ACCESS, true, DUPLICATE_SAME_ACCESS);
	DuplicateHandle(GetCurrentProcess(), timerHandle, pi1->hProcess, NULL, DUPLICATE_SAME_ACCESS, true, DUPLICATE_SAME_ACCESS);
	myGetch();
	if (!CloseHandle(timerHandle)) cout << "Timer deletion failed" << endl; else cout << "Timer successfully deleted!" << endl;
	if (!CloseHandle(mutexHandle)) cout << "Mutex deletion failed" << endl; else cout << "Mutex successfully deleted!" << endl;
	if (!CloseHandle(semaphoreHandle)) cout << "Semaphore deletion failed" << endl; else cout << "Semaphore successfully deleted!" << endl;
	myGetch();
	return 0;
}