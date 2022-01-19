#include "librace.h" 
#include "model-assert.h"
void assume(int);

atomic_int x;
atomic_int y;
atomic_int z;

int res;

void *thread_1(void *unused)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	assume(a == 1);

	atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);

	int b = atomic_load_explicit(__FILE__, __LINE__, &z, memory_order_relaxed);
	assume(b == 0);
	;
}

void *thread_2(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &z, 1, memory_order_relaxed);

	atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);

	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	/* assume(c == 1); */
	;
}

void *thread_3(void *unused)
{
	int d = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	if (d == 1)
		atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);

	assume(d == 1);
	res = d; /* Used in an MODEL_ASSERTion in user_user_user_main(), to make sure it's invalid */
	;
}
