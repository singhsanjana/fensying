#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	for (int i = 1u; i <= 42; i++)
		atomic_store_explicit(__FILE__, __LINE__, &x, i, memory_order_release);
	;
}

void *thread_2(void *unused)
{
	for (int i = 1u; i <= 42; i++)
		atomic_store_explicit(__FILE__, __LINE__, &y, i, memory_order_release);
	;
}

void *thread_3(void *unused)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	assume(a == 42);
	int b = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
	assume(b == 0);
	;
}

void *thread_4(void *unused)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
	assume(a == 0);
	int b = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
	assume(b == 42);
	;
}
