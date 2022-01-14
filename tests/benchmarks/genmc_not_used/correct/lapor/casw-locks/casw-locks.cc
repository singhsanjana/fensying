#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex l;

int x;
int idx[N];

void *thread_n(void *arg)
{
	int r = 0, val = *((int *) arg);

	l.lock();

	if (x == r)
		x = 1;

	l.unlock();

	l.lock();
	x = val + 3;
	l.unlock();
	;
}
