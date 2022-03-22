#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

atomic_int x;
atomic_int y;
atomic_int f;
int a,b,c;

static void fn1(void *obj)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
}

static void fn2(void *obj)
{
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
    a = r1;
    if (r1 == 1) {
        // atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &f, 0, memory_order_acq_rel);
        b = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
    }
}

static void fn3(void *obj)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
    // atomic_thread_fence(__FILE__, __LINE__, memory_order_seq_cst);
    atomic_fetch_add_explicit(__FILE__, __LINE__, &f, 0, memory_order_acq_rel);
    c = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2, t3;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&f, 0);
	a = b = c = 0;

	thrd_create(&t1, (thrd_start_t)&fn1, NULL);
	thrd_create(&t2, (thrd_start_t)&fn2, NULL);
	thrd_create(&t3, (thrd_start_t)&fn3, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);

    MODEL_ASSERT(! (a==1 && b==0 && c==0) );

	return 0;
}
