#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	if (r1 >= 0)
		atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int r2 = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, r2, memory_order_relaxed);
	;
}
