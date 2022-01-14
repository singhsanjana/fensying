#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#ifndef N
# define N 2
#endif

std::mutex l;
int x;

int idx[N];

void *threadn(void *arg)
{
	int i = *((int *) arg);

	l.lock();
	int r = x;
	x = i + 1;
	l.unlock();
	;
}
