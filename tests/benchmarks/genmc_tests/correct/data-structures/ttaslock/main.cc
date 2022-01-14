#include "librace.h" 
#include "model-assert.h"
#include "ttaslock.cc"

#ifndef N
# define N 2
#endif

int shared;
ttaslock_t lock;

void *thread_n(void *arg)
{
	intptr_t index = ((intptr_t) arg);

	ttaslock_acquire(&lock);
	shared = index;
	int r = shared;
	MODEL_ASSERT(r == index);
	ttaslock_release(&lock);
	;
}
