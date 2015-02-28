#include <iostream>
#include <windows.h>
#include "util.h"
using namespace std;
int Proc1()
{
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -100000000LL;
	HWND hwnd = GetActiveWindow();
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

	while (getchar() != '\n') continue;
	if (!CloseHandle(timerHandle)) cout << "Timer deletion failed" << endl; else cout << "Timer successfully deleted!" << endl;
	if (!CloseHandle(mutexHandle)) cout << "Mutex deletion failed" << endl; else cout << "Mutex successfully deleted!" << endl;
	if (!CloseHandle(semaphoreHandle)) cout << "Semaphore deletion failed" << endl; else cout << "Semaphore successfully deleted!" << endl;
	while (getchar() != '\n') continue;
	return 0;
}