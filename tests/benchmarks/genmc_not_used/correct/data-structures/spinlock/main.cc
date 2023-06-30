#include "librace.h" 
#include "model-assert.h"
#include "spinlock.cc"

#ifndef N
# define N 2
#endif

int shared;
spinlock_t lock;

void *thread_n(void *arg)
{
	intptr_t index = ((intptr_t) arg);

	spinlock_acquire(&lock);
	shared = index;
	int r = shared;
	MODEL_ASSERT(r == index);
	spinlock_release(&lock);
	;
}
