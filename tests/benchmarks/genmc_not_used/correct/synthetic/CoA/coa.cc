#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;
atomic_int z;

int idx[N];

void *thread_one(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	;
}

void *thread_two(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
	atomic_load_explicit(__FILE__, __LINE__, &z, memory_order_acquire);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	;
}

void *thread_n(void *arg)
{
	int i = *((int *) arg);

	atomic_store_explicit(__FILE__, __LINE__, &y, i, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &z, i, memory_order_release);
	;
}
