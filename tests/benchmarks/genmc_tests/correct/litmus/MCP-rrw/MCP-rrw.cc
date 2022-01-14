#include "librace.h" 
#include "model-assert.h"
atomic_int x;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	;
}

void *thread_2(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
	;
}

void *thread_3(void *unused)
{
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	;
}
