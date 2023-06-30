#include "librace.h" 
#include "model-assert.h"
void assume(int);

atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	assume(a == 2);

	y = 1;
	;
}

void *thread_2(void *unused)
{
	y = 2;
	;
}

void *thread_3(void *unused)
{
	int b = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	assume(b == 2);

	atomic_store_explicit(__FILE__, __LINE__, &x, b, memory_order_relaxed);
	;
}
