#include "librace.h" 
#include "model-assert.h"

atomic_int x;
atomic_int y;
atomic_int c;

void *thread_one(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
	if (!atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed)) {
		atomic_store_explicit(__FILE__, __LINE__, &c, 1, memory_order_relaxed);
		MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &c, memory_order_relaxed) == 1);
	}
	
	return NULL;
}

void *thread_two(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	if (!atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed)) {
		atomic_store_explicit(__FILE__, __LINE__, &c, 0, memory_order_relaxed);
		MODEL_ASSERT(atomic_load_explicit(__FILE__, __LINE__, &c, memory_order_relaxed) == 0);
	}

	return NULL;
}
