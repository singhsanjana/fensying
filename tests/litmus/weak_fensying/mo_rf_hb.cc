// optimal sol'n : 1 rel b/w 28,19 and 1 acq b/w 16,17

#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

atomic_int x;
atomic_int y;
int a, b, c;

static void fn1(void *obj)
{
	a=atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
}

static void fn2(void *obj)
{
	b=atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
    atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_relaxed);
}

static void fn3(void *obj)
{
	c=atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
    atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2, t3;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	a = b = c = 0;

	thrd_create(&t1, (thrd_start_t)&fn1, NULL);
	thrd_create(&t2, (thrd_start_t)&fn2, NULL);
	thrd_create(&t3, (thrd_start_t)&fn3, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);

	MODEL_ASSERT(!(a==1 && b==1 && c==2));

	return 0;
}
