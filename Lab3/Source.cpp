#include <thread>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "crit.h"
#include <intsafe.h>

#define BOUND 50

using namespace std;

CritSec cs;
Event e("mnx",false);
Event ea("autores", true);

bool is_prime(long long int n)
{
	long long int i, count = 0;
	if (n == 1 || n == 2)
		return true;
	if (n % 2 == 0)
		return false;
	for (i = 1; i <= n; i++)
	{
		if (n%i == 0 && count >= 2)
			return false;
		else if (n%i == 0)
			count++;
	}
	if (count == 2)
		return true;
	else
		return false;
}

void thrFunNeg()
{
	CritLock c(cs.get());
	for (int i = -BOUND; i < 0; i++)
	{
		cout << i << endl;
	}
}

void thrFunPos()
{
	CritLock c(cs.get());
	for (int i = 0; i < BOUND + 1; i++)
	{
		cout << i << endl;
	}
	e.signal(1);
}

void thrFunPrime()
{
	for (long long int i = 0; i < LONGLONG_MAX; i++)
	{
		if (is_prime(i))
		{
			cout << "Prime: " << i << endl;
		}
		e.wait();
	}
}
void thrFunFour()
{
	for (long long int i = 0; i < LONGLONG_MAX; i++)
	{
		if (is_prime(i))
		{
			cout << "Prime: " << i << endl;
		}
		ea.wait();
	}
}

void Getch()
{
	while (getchar() != '\n');
}

void main()
{
	thread t1(thrFunNeg);
	SetThreadPriority(t1.native_handle(), THREAD_PRIORITY_ABOVE_NORMAL);
	thread t2(thrFunPos);
	SetThreadPriority(t2.native_handle(), THREAD_PRIORITY_BELOW_NORMAL);
	t1.detach();
	t2.detach();
	e.wait();
	Getch();
	e.set();
	thread t3(thrFunPrime);
	t3.detach();
	int prior;
	Getch();
	e.reset();
	cout << "Thread paused" << endl;
	cout << "Enter new thread priority: ";
	cin >> prior;
	Getch();
	SetThreadPriority(t3.native_handle(), prior);
	cout << "Thread priority set to "<<prior << endl;
	Getch();
	e.set();
	Getch();
	e.reset();
	TerminateThread(t3.native_handle(), 0);
	cout << "Thread terminated" << endl;
	Getch();
	thread t4(thrFunFour);
	t4.detach();
	Getch();
	ea.set();
	Getch();
}