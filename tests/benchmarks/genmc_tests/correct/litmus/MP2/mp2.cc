#include "librace.h" 
#include "model-assert.h"
atomic_int a;
atomic_int b;
atomic_int c;
atomic_int d;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &a, 1, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &c, 1, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_acq_rel);
	atomic_store_explicit(__FILE__, __LINE__, &b, 1, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &d, 1, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_acq_rel);
	int r2 = atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_relaxed);
	;
}

void *thread_3(void *unused)
{
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &d, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_acq_rel);
	int r2 = atomic_load_explicit(__FILE__, __LINE__, &c, memory_order_relaxed);
	;
}
