#include "crit.h"
#include <exception>
using namespace std;

Event::Event(const char* name, bool autoReset)
{
	_name = name;
	_hev = CreateEvent(NULL, !autoReset, FALSE, name);
	if (_hev == NULL) throw new bad_alloc;
}

Event::~Event()
{
	CloseHandle(_hev);
}

void Event::signal(int deltaT)
{
	SetEvent(_hev);
	Sleep(deltaT);
	ResetEvent(_hev);
}

void Event::wait()
{
	WaitForSingleObject(_hev, INFINITE);
}

void Event::set()
{
	SetEvent(_hev);
}

void Event::reset()
{
	ResetEvent(_hev);
}

CritSec::CritSec()
{
	InitializeCriticalSection(&_cs);
}

CritSec::~CritSec()
{
	DeleteCriticalSection(&_cs);
}

CRITICAL_SECTION* CritSec::get()
{
	return &_cs;
}

CritLock::CritLock(CRITICAL_SECTION* cs)
	{
		p_cs = cs;
		EnterCriticalSection(p_cs);
	}
CritLock::~CritLock()
	{
		LeaveCriticalSection(p_cs);
	}