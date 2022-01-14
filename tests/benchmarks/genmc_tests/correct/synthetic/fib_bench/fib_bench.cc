#include "librace.h" 
#include "model-assert.h"
atomic_int x = ATOMIC_VAR_INIT(1);
atomic_int y = ATOMIC_VAR_INIT(1);

#ifndef NUM
#define NUM 5
#endif

void *thread_1(void* arg)
{
	for (int i = 0; i < NUM; i++) {
		int prev_x = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
		int prev_y = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
		atomic_store_explicit(__FILE__, __LINE__, &x, prev_x + prev_y, memory_order_release);
	}
	;
}

void *thread_2(void* arg)
{
	for (int i = 0; i < NUM; i++) {
		int prev_x = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
		int prev_y = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
		atomic_store_explicit(__FILE__, __LINE__, &y, prev_x + prev_y, memory_order_release);
	}
	;
}

void *thread_3(void *arg)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire) > 144 ||
	    atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire) > 144)
		MODEL_ASSERT(0);
	;
}
