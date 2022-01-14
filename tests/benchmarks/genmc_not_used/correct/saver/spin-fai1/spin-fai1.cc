#include "librace.h" 
#include "model-assert.h"
atomic_int x;

void *thread_1(void *unused)
{
	while (true) {
		atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
		atomic_fetch_sub_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	}
	;
}

void *thread_2(void *unused)
{
	while (true) {
		atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
		atomic_fetch_add_explicit(__FILE__, __LINE__, &x, -1, memory_order_relaxed);
	}
	;
}

void *thread_3(void *unused)
{
	while (true) {
		atomic_fetch_sub_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
		atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	}
	;
}
