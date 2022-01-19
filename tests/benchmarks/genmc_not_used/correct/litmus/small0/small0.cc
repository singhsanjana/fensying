#include "librace.h" 
#include "model-assert.h"
atomic_int x;

void *thread_one(void *arg)
{
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	;
}

void *thread_two(void *arg)
{
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	;
}

void *thread_three(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
	;
}
