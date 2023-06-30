#include "librace.h" 
#include "model-assert.h"
atomic_int x;
int idx[N];

void *thread_n(void *arg)
{
	int v = *((int *) arg);

	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, v, memory_order_release);
	;
}
