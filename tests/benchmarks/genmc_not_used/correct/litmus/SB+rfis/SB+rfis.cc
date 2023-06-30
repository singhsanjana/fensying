#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
	atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_seq_cst);
	;
}

void *thread_2(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_seq_cst);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst);
	;
}
