#include "librace.h" 
#include "model-assert.h"
#include "ticketlock.cc"

#ifndef N
# define N 2
#endif

int shared;
ticketlock_t lock;

void *thread_n(void *arg)
{
	intptr_t index = ((intptr_t) arg);

	ticketlock_acquire(&lock);
	shared = index;
	int r = shared;
	MODEL_ASSERT(r == index);
	ticketlock_release(&lock);
	;
}
