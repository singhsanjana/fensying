#include "librace.h" 
#include "model-assert.h"
#ifndef NUM
# define NUM 4
#endif

atomic_int x;
atomic_int a[8];

void *thread_1(void *unused)
{
	for (int i = 1u; i <= 42; i++)
		x = i;
	;
}

void *thread_2(void *unused)
{
	assume(x == 42);
	;
}

void *thread_3(void *unused)
{
	for (int i = 0u; i < NUM; i++) {
		for (int j = 0u; j < 8; j++)
			atomic_store_explicit(__FILE__, __LINE__, &a[j], i, memory_order_relaxed);
	}
	;
}

void *thread_4(void *unused)
{
	for (int i = 0u; i < 8; i++)
		atomic_load_explicit(__FILE__, __LINE__, &a[i], memory_order_relaxed);
	;
}
