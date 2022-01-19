#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;
atomic_int z1;
atomic_int z2;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_seq_cst);
	if (!atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_seq_cst)) {
		atomic_store_explicit(__FILE__, __LINE__, &z1, 1, memory_order_relaxed);
	}
	;
}

void *thread_2(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_seq_cst);
	if (!atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_seq_cst)) {
		atomic_store_explicit(__FILE__, __LINE__, &z2, 1, memory_order_relaxed);
	}
	;
}

void *thread_3(void *unused)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &z1, memory_order_relaxed) &&
	    atomic_load_explicit(__FILE__, __LINE__, &z2, memory_order_relaxed)) {
		for (int i = 0; i < N; i++) {
			atomic_store_explicit(__FILE__, __LINE__, &x, i, memory_order_relaxed);
		}
	}
	;
}

void *thread_4(void *unused)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &z1, memory_order_relaxed) &&
	    atomic_load_explicit(__FILE__, __LINE__, &z2, memory_order_relaxed)) {
		for (int i = 0; i < N; i++) {
			atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
		}
	}
	;
}
