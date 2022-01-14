#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int idx[N+1];

void *thread_writer(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 42, memory_order_release);
	;
}

void *thread_reader(void *arg)
{
	int r = *((int *) arg);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	;
}
