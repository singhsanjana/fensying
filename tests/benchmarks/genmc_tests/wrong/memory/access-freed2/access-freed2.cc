#include "librace.h" 
#include "model-assert.h"
_Atomic(atomic_int *) p;

int *x[2];

void *thread1(void *unused)
{
	free(p);
	;
}

void *thread2(void *unused)
{
	atomic_int *a = atomic_load_explicit(__FILE__, __LINE__, &p, memory_order_acquire);
	atomic_store_explicit(__FILE__, __LINE__, a, 42, memory_order_relaxed);
	;
}
