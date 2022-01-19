#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;
atomic_int z;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x,1,memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
	int r0 = atomic_load_explicit(__FILE__, __LINE__, &y,memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &y,1,memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &z,1,memory_order_release);
	;
}

void *thread_3(void *unused)
{
	int r0 = atomic_load_explicit(__FILE__, __LINE__, &z,memory_order_acquire);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &x,memory_order_relaxed);
	;
}
