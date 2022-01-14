#include "librace.h" 
#include "model-assert.h"
atomic_int x;

void *thread_0(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	;
}

void *thread_n(void *arg)
{
	int r = *((int *) arg);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, r, memory_order_release);
	;
}
