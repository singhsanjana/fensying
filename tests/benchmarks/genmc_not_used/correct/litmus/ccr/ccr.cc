#include "librace.h" 
#include "model-assert.h"
atomic_int x;

void *thread_1(void *unused)
{
	int r = 0;
	atomic_compare_exchange_strong_explicit(__FILE__, __LINE__&x, &r, 42, memory_order_relaxed,
						            memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int r = 0;
	atomic_compare_exchange_strong_explicit(__FILE__, __LINE__&x, &r, 17, memory_order_relaxed,
						            memory_order_relaxed);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	;
}
