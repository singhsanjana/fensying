#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;
atomic_int z;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &z, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, a - 1, memory_order_relaxed);
	int b = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &y, b, memory_order_relaxed);
	;
}

void *thread_3(void *unused)
{
	int c = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &z, c, memory_order_relaxed);
	;
}
