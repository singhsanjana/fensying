#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	int r = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	if (!r)
		atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	else
		atomic_store_explicit(__FILE__, __LINE__, &y, 2, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int r = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	;
}
