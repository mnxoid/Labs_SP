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
	else if (argc > 1 && !strcmp(args[1], "Process4"))//ShellExecute passes parameters in a different way
	{
		return Proc4();
	}
	else if (!strcmp(args[0], "Process5"))
	{
		return Proc5();
	}
	else
	{
// 		SECURITY_ATTRIBUTES  sa;
// 
// 		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
// 		sa.bInheritHandle = TRUE;
// 
// 		// Call function to set the DACL. The DACL
// 		// is set in the SECURITY_ATTRIBUTES 
// 		// lpSecurityDescriptor member.
// 		if (!CreateMyDACL(&sa))
// 		{
// 			// Error encountered; generate message and exit.
// 			printf("Failed CreateMyDACL\n");
// 			exit(1);
// 		}
		cout << "Tap enter to create first process";
		myGetch();
		CreateSelfProcess(args[0], "Process1");// , &sa);
		cout << "Tap enter to create second process";
		myGetch();
		CreateSelfProcess(args[0], "Process2");// , &sa);
		cout << "Tap enter to create third process";
		myGetch();
		CreateSelfProcess(args[0], "Process3");// , &sa);
		cout << "Tap enter to create fourth process";
		myGetch();
		ShellExecute(NULL, "runas", args[0], "Process4", NULL, SW_SHOWNORMAL);//Need to run as administrator
		//CreateSelfProcess(args[0], "Process4");
	}
	myGetch();
	return 0;
}