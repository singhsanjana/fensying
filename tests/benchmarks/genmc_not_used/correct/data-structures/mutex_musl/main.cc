#include "librace.h" 
#include "model-assert.h"
#include "mutex_musl.cc"

#ifndef N
# define N 2
#endif

int shared;
mutex_t mutex;

void *thread_n(void *arg)
{
	intptr_t index = ((intptr_t) arg);

	mutex_lock(&mutex);
	shared = index;
	int r = shared;
	MODEL_ASSERT(r == index);
	mutex_unlock(&mutex);
	;
}
