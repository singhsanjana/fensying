#include "librace.h" 
#include "model-assert.h"
atomic_int x;

void *thread_1(void *unused)
{
	int r = 1;

	atomic_compare_exchange_strong_explicit(__FILE__, __LINE__&x, &r, 2, memory_order_relaxed,
						memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	;
}

void *thread_3(void *unused)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	;
}
