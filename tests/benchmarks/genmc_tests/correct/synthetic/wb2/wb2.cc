#include "librace.h" 
#include "model-assert.h"
#define mo_rlx memory_order_relaxed

atomic_int x;

void assume(int);

void *thread1(void *arg)
{
	int a = atomic_load_explicit(__FILE__, __LINE__, &x, mo_rlx);
	assume(a == 5);
	int b = atomic_load_explicit(__FILE__, __LINE__, &x, mo_rlx);
	;
}

void *thread2(void *arg)
{
	int c = atomic_load_explicit(__FILE__, __LINE__, &x, mo_rlx);
	assume(c == 4);
	atomic_store_explicit(__FILE__, __LINE__, &x, 5, mo_rlx);
	;
}

void *thread3(void *arg)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 3, mo_rlx);
	atomic_store_explicit(__FILE__, __LINE__, &x, 4, mo_rlx);
	;
}

void *thread4(void *arg)
{
	int d = atomic_fetch_add_explicit(__FILE__, __LINE__, &x, 10, mo_rlx);
	assume(d == 3);
	;
}
