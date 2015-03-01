#include <iostream>
#include <windows.h>
#include "util.h"
using namespace std;

int Proc4()
{
	LPBYTE storeHere = (LPBYTE)calloc(1, 256);
	if(GetKeyData(HKEY_LOCAL_MACHINE, "Software\\Mnxoid", "Lab1_Proc4", storeHere, 256))
	{
		cout << "Sorry, but I won't run any more" << endl;
		myGetch();
		return 0;
	}
	else
	{
		if (SetKeyData(HKEY_LOCAL_MACHINE, "Software\\Mnxoid", REG_SZ, "Lab1_Proc4", (LPBYTE)"Any text", strlen("Any text")))
		{
			cout << "You're lucky!";
			myGetch();
		}
		else
		{
			cout << "Error writing key!";
			myGetch();
		}
	}
	return 0;
}