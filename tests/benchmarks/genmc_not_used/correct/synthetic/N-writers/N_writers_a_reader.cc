#include "librace.h" 
#include "model-assert.h"
atomic_int x;
int idx[N + 1];

void *threadW(void *arg)
{
	int i = *((int *) arg);
	atomic_store_explicit(__FILE__, __LINE__, &x, i, memory_order_release);
	;
}

void *threadR(void *unused)
{
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	;
}
