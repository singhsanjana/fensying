#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	atomic_store(__FILE__, __LINE__, &x, 1);
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	;
}

void *thread_2(void *unused)
{
	atomic_fetch_add(__FILE__, __LINE__, &y, 1);
	atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	;
}

void *thread_3(void *unused)
{
	atomic_store(__FILE__, __LINE__, &y, 3);
	atomic_load(__FILE__, __LINE__, &x);
	;
}
