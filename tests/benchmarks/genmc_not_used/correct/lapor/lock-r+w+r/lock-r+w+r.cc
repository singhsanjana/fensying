#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex l;

int x;

void *thread1(void *unused)
{
	l.lock();
	int r = x;
	l.unlock();
	;
}

void *thread2(void *unused)
{
	l.lock();
	x = 42;
	l.unlock();
	;
}

void *thread3(void *unused)
{
	l.lock();
	int r = x;
	l.unlock();
	;
}
