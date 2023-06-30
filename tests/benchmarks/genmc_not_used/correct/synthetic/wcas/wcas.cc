#include "librace.h" 
#include "model-assert.h"
atomic_int x;

void *thread_0(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
	;
}

void *thread_n(void *unused)
{
	int r = 2;
	atomic_compare_exchange_strong_explicit(__FILE__, __LINE__&x, &r, 3, memory_order_relaxed,
						memory_order_relaxed);
	;
}
