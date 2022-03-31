#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

atomic_int x;
atomic_int y;
int a, b;

static void fn1(void *obj)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_seq_cst);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_release);
	a = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
}

static void fn2(void *obj)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_seq_cst);
	atomic_thread_fence(__FILE__, __LINE__, memory_order_acquire);
	b = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	a = b = 0;

	thrd_create(&t1, (thrd_start_t)&fn1, NULL);
	thrd_create(&t2, (thrd_start_t)&fn2, NULL);

	thrd_join(t1);
	thrd_join(t2);

    MODEL_ASSERT(! (a==0 && b==0) );

	return 0;
}
