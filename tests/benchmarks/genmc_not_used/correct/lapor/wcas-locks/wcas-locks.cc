#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex l;
int x;

void *thread_0(void *unused)
{
	l.lock();
	x = 2;
	l.unlock();
	;
}

void *thread_n(void *unused)
{
	l.lock();
	if (x == 2)
		x = 3;
	l.unlock();
	;
}
