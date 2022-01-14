#include "librace.h" 
#include "model-assert.h"
/* Test case by: Jonas Oberhauser */

atomic_int x;
atomic_int y;
atomic_int z;

void *runA(void *p)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
#ifdef USE_STORE
	atomic_store_explicit(__FILE__, __LINE__, &z, 1, memory_order_release);
#else
	atomic_exchange_explicit(__FILE__, __LINE__, &z, 1, memory_order_release);
#endif
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	;
}

void *runB(void *q)
{
	int b = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	;
}
