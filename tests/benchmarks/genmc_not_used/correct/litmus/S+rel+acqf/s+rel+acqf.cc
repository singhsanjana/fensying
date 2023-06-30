#include "librace.h" 
#include "model-assert.h"
int x;
atomic_int y;

void *thread_1(void *arg)
{
	x = 1;
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	;
}

void *thread_2(void *arg)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed)) {
		atomic_thread_fence(__FILE__, __LINE__, memory_order_acquire);
		x = 2;
	}
	;
}
