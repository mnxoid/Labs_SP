#include <iostream>
#include <windows.h>
#include "util.h"
using namespace std;
int Proc2()
{
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -100000000LL;
	HWND hwnd = GetActiveWindow();
	HANDLE timerHandle = OpenWaitableTimer(TIMER_ALL_ACCESS, TRUE, "MnxoidTimer");
	if (timerHandle != NULL)
	{
		cout << "Timer opened!" << endl;
	}
	else {
		cout << "Error opening timer (" << GetLastError() << ")" << endl;
	}
	HANDLE mutexHandle = OpenMutex(MUTEX_ALL_ACCESS, TRUE, "MnxoidMutex");
	if (timerHandle != NULL)
	{
		cout << "Mutex opened!" << endl;
	}
	else {
		cout << "Error opening mutex (" << GetLastError() << ")" << endl;
	}
	HANDLE semaphoreHandle = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "MnxoidSemaphore");
	if (semaphoreHandle != NULL)
	{
		cout << "Semaphore opened!" << endl;
	}
	else {
		cout << "Error opening semaphore (" << GetLastError() << ")" << endl;
	}
	if (WaitForSingleObject(timerHandle, INFINITE) != WAIT_OBJECT_0)
		cout << "WaitForSingleObject failed (" << GetLastError() << ")" << endl;
	else cout << "Timer was signaled." << endl;

	myGetch();
	if (!CloseHandle(timerHandle)) cout << "Timer deletion failed" << endl; else cout << "Timer successfully deleted!" << endl;
	if (!CloseHandle(mutexHandle)) cout << "Mutex deletion failed" << endl; else cout << "Mutex successfully deleted!" << endl;
	if (!CloseHandle(semaphoreHandle)) cout << "Semaphore deletion failed" << endl; else cout << "Semaphore successfully deleted!" << endl;
	myGetch();
	return 0;
}