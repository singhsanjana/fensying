#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#ifndef N
# define N 2
#endif

std::mutex l;
int x;
int y;

int idx[N];

void *threadn(void *arg)
{
	int i = *((int *) arg);

	l.lock();
	x = i;
	y = i;
	l.unlock();
	;
}
