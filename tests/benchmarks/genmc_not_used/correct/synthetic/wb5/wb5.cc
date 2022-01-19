#include "librace.h" 
#include "model-assert.h"
#define mo_rlx memory_order_relaxed

atomic_int x;

void assume(int);

void *thread1(void *arg)
{

	int a = atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, mo_rlx);
	assume(a == 0);
	;
}

void *thread2(void *arg)
{
	int b = atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 1, mo_rlx);
	assume(b == 10);
	atomic_load_explicit(__FILE__, __LINE__, &x, mo_rlx);
	;
}

void *thread3(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 10, mo_rlx);
	;
}
