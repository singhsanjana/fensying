#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;
atomic_int z;

void *thread_n(void *unused)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	atomic_fetch_add_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	atomic_fetch_add_explicit(__FILE__, __LINE__, &z, 1, memory_order_relaxed);
	;
}
