#include <iostream>
#include <windows.h>
using namespace std;
void CreateSelfProcess(LPCTSTR mname,LPTSTR pname)
{
	STARTUPINFO* si1=new STARTUPINFO;
	PROCESS_INFORMATION* pi1=new PROCESS_INFORMATION;

	ZeroMemory(si1, sizeof(*si1));
	si1->cb = sizeof(*si1);
	ZeroMemory(pi1, sizeof(*pi1));
	CreateProcess(mname, pname, NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, si1, pi1);
}
int main(int argc, char* args[])
{
	if (argc < 2)
	{
		CreateSelfProcess(args[0], "Process1");
		CreateSelfProcess(args[0], "Process2");
	}
	else {
		if (!strcmp(args[1],"Process1"))
		{
			LARGE_INTEGER liDueTime;
			liDueTime.QuadPart = -100000000LL;
			HWND hwnd = GetActiveWindow();
			HANDLE timerHandle = CreateWaitableTimerEx(NULL, "MnxoidTimer1", 0, TIMER_ALL_ACCESS);
			if (timerHandle != NULL)
			{
				cout << "Timer created!" << endl;
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
			while (getchar() != '\n') continue;
			return 0;
		}
		else if (!strcmp(args[1], "Process2"))
		{
			LARGE_INTEGER liDueTime;
			liDueTime.QuadPart = -100000000LL;
			HWND hwnd = GetActiveWindow();
			HANDLE timerHandle = OpenWaitableTimer(TIMER_ALL_ACCESS, TRUE, "MnxoidTimer1");
			if (timerHandle != NULL)
			{
				cout << "Timer created!" << endl;
			}
			else {
				cout << "Error opening timer (" << GetLastError() << ")" << endl;
			}
			if (WaitForSingleObject(timerHandle, INFINITE) != WAIT_OBJECT_0)
				cout << "WaitForSingleObject failed (" << GetLastError() << ")" << endl;
			else cout << "Timer was signaled." << endl;

			while (getchar() != '\n') continue;
			if (!CloseHandle(timerHandle)) cout << "Timer deletion failed" << endl; else cout << "Timer successfully deleted!" << endl;
			while (getchar() != '\n') continue;
			return 0;
		}
	}
}