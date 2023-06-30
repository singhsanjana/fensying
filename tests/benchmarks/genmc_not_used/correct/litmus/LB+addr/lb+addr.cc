#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y[2];

void *thread_1(void *unused)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	int b = atomic_load_explicit(__FILE__, __LINE__, y + a, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, y, 1, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int r = atomic_load_explicit(__FILE__, __LINE__, y, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, r, memory_order_release);
	;
}
