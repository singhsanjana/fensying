#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;
atomic_int z;

void *thread_1(void *unused)
{
	atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
	atomic_fetch_add_explicit(__FILE__, __LINE__, &z, 1, memory_order_acq_rel);
	;
}

void *thread_2(void *unused)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &z, 1, memory_order_acq_rel);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	;
}

void *thread_3(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	;
}
