#include "librace.h" 
#include "model-assert.h"
#define mo_rlx memory_order_relaxed

atomic_int x;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, mo_rlx);
	;
}

void *thread_2(void *unused)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &x, mo_rlx);
	int b = atomic_load_explicit(__FILE__, __LINE__, &x, mo_rlx);
	int c = atomic_load_explicit(__FILE__, __LINE__, &x, mo_rlx);
	MODEL_ASSERT(!(a == 1 && b == 2 && c == 1));
	;
}

void *thread_3(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 2, mo_rlx);
	;
}
