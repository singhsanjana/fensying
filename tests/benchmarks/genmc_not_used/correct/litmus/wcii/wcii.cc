#include "librace.h" 
#include "model-assert.h"
atomic_int x;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 5, memory_order_release);
	;
}

void *thread_2(void *unused)
{
	int expected = 1;
	atomic_compare_exchange_strong_explicit(__FILE__, __LINE__&x, &expected, 2,
						memory_order_relaxed,
						memory_order_relaxed);
	;
}

void *thread_3(void *unused)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	;
}

void *thread_4(void *unused)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	;
}
