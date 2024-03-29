#include "librace.h" 
#include "model-assert.h"
atomic_int x;
int idx[N];

void *thread_n(void *arg)
{
	int r = 0, val = *((int *) arg);

	atomic_compare_exchange_strong_explicit(__FILE__, __LINE__&x, &r, 1, memory_order_relaxed,
						memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, val + 3, memory_order_release);
	;
}
