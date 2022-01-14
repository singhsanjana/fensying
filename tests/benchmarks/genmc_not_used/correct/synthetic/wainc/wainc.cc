#include "librace.h" 
#include "model-assert.h"
atomic_int x;

void *thread_0(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 42, memory_order_release);
	;
}

void *thread_n(void *unused)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	;
}
