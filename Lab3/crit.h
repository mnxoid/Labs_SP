#include <windows.h>

class Event
{
public:
	Event(const char* name, bool autoReset);
	~Event();
	void signal(int deltaT);
	void wait();
	void set();
	void reset();
private:
	HANDLE _hev;
	const char* _name;
};

class CritSec
{
public:
	CritSec();
	~CritSec();
	CRITICAL_SECTION* get();
private:
	CRITICAL_SECTION _cs;
};

class CritLock
{
public:
	CritLock(CRITICAL_SECTION* cs);
	~CritLock();
private:
	CRITICAL_SECTION* p_cs;
};