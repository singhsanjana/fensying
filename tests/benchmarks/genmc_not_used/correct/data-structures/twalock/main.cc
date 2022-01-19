#include "librace.h" 
#include "model-assert.h"
#ifndef N
# define N 2
#endif

#include "twalock.cc"

int shared;

TWALOCK_ARRAY_DECL;
twalock_t lock = TWALOCK_INIT();

void *thread_n(void *arg)
{
	intptr_t index = ((intptr_t) arg);

	twalock_acquire(&lock);
	shared = index;
	int r = shared;
	MODEL_ASSERT(r == index);
	twalock_release(&lock);
	;
}
