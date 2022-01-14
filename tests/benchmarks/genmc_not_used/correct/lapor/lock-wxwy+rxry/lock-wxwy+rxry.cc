#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex l;

int x;
int y;

void *thread1(void *unused)
{
	l.lock();
	x = 42;
	y = 42;
	l.unlock();
	;
}

void *thread2(void *unused)
{
	l.lock();
	int a = x;
	int b = y;
	l.unlock();
	;
}
