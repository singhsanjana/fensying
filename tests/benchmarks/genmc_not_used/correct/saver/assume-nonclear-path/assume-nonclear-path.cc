#include "librace.h" 
#include "model-assert.h"
#ifndef N
# define N 42
#endif

atomic_int x;
atomic_int y;

void assume(int);

void *thread_1(void *unused)
{
	for (int i = 1u; i <= N; i++)
		atomic_store_explicit(__FILE__, __LINE__, &x, i, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	int a = 0;
	int b = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);

	if (a < N)
		atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);

	assume(b == N);

	;
}
