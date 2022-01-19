#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#ifndef N
# define N 2
#endif

std::mutex l;
int x;

void *threadn(void *unused)
{
	l.lock();
	int r = x;
	l.unlock();
	;
}
