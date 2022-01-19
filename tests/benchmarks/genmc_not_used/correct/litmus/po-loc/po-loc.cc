#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	int r0 = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_acq_rel);
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int r2 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &y, r2, memory_order_relaxed);
	;
}
