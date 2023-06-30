#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex l;

int x, y;

void *thread1(void *unused)
{
	l.lock();
	x = 42;
	l.unlock();
	;
}

void *thread2(void *unused)
{
	l.lock();
	y = 42;
	l.unlock();
	;
}

void *thread3(void *unused)
{
	l.lock();
	if (x == 42 && y == 42)
		; /* printf("hooray\n"); */
	l.unlock();
	;
}
