#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_acq_rel);
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int r1 = atomic_exchange_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	int r2 = atomic_exchange_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	;
}
/* exists 1:r1=1 /\ 1:r2=0 */
