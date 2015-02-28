#include <iostream>
#include <windows.h>
#include "util.h"
using namespace std;

int main(int argc, char* args[])
{
	cout << "Call params:" << endl;
	cout << "argc: " << argc << endl;
	cout << "args[0]: " << args[0] << endl;
	if (!strcmp(args[0], "Process1"))
	{
		return Proc1();
	}
	else if (!strcmp(args[0], "Process2"))
	{
		return Proc2();
	}
	else if (!strcmp(args[0], "Process3"))
	{
		return Proc3();
	}
	else
	{
		cout << "Tap enter to create first process";
		while (getchar() != '\n') continue;
		CreateSelfProcess(args[0], "Process1");
		cout << "Tap enter to create second process";
		while (getchar() != '\n') continue;
		CreateSelfProcess(args[0], "Process2");
		cout << "Tap enter to create third process";
		while (getchar() != '\n') continue;
		CreateSelfProcess(args[0], "Process3");
	}
	while (getchar() != '\n') continue;
	return 0;
}