#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

atomic_int x;
atomic_int y;
int a,b,c,d;

static void fn1(void *obj)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
}

static void fn2(void *obj)
{
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
    a = r1;
    if (r1 == 1) {
        b = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
    }
}

static void fn3(void *obj)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
}

static void fn4(void *obj)
{
	int r1 = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
    c = r1;
    if (r1 == 1) {
        d = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire);
    }
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2, t3, t4;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	a = b = c = d = 0;

	thrd_create(&t1, (thrd_start_t)&fn1, NULL);
	thrd_create(&t2, (thrd_start_t)&fn2, NULL);
	thrd_create(&t3, (thrd_start_t)&fn3, NULL);
	thrd_create(&t4, (thrd_start_t)&fn4, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	thrd_join(t4);

    MODEL_ASSERT(! (a==1 && b==0 && c==1 && d==0) );

	return 0;
}
