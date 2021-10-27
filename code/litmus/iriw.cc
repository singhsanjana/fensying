#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

atomic_int x;
atomic_int y;
atomic_int a;
atomic_int b;
atomic_int c;
atomic_int d;

static void fn1(void *obj)
{
	atomic_store_explicit(__LINE__, &x, 1, memory_order_release);
}

static void fn2(void *obj)
{
	int r1 = atomic_load_explicit(__LINE__, &x, memory_order_acquire);
    atomic_store_explicit(__LINE__, &a, r1, memory_order_relaxed);
    if (r1 == 1) {
        int r2 = atomic_load_explicit(__LINE__, &y, memory_order_acquire);
        atomic_store_explicit(__LINE__, &b, r2, memory_order_relaxed);
    }
}

static void fn3(void *obj)
{
	atomic_store_explicit(__LINE__, &y, 1, memory_order_release);
}

static void fn4(void *obj)
{
	int r1 = atomic_load_explicit(__LINE__, &y, memory_order_acquire);
    atomic_store_explicit(__LINE__, &c, r1, memory_order_relaxed);
    if (r1 == 1) {
        int r2 = atomic_load_explicit(__LINE__, &x, memory_order_acquire);
        atomic_store_explicit(__LINE__, &d, r2, memory_order_relaxed);
    }
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2, t3, t4;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&a, 0);
	atomic_init(&b, 0);
	atomic_init(&c, 0);
	atomic_init(&d, 0);

	thrd_create(&t1, (thrd_start_t)&fn1, NULL);
	thrd_create(&t2, (thrd_start_t)&fn2, NULL);
	thrd_create(&t3, (thrd_start_t)&fn3, NULL);
	thrd_create(&t4, (thrd_start_t)&fn4, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);

    int ra = atomic_load_explicit(__LINE__, &a, memory_order_relaxed);
    int rb = atomic_load_explicit(__LINE__, &b, memory_order_relaxed);
    int rc = atomic_load_explicit(__LINE__, &c, memory_order_relaxed);
    int rd = atomic_load_explicit(__LINE__, &d, memory_order_relaxed);

    MODEL_ASSERT(! (ra==1 && rb==0 && rc==1 && rd==0) );

	return 0;
}
