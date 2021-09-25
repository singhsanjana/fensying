#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

atomic_int x;
atomic_int y;

static void a(void *obj)
{
	atomic_store_explicit(__LINE__, &x, 1, memory_order_release);
	atomic_store_explicit(__LINE__, &x, 2, memory_order_relaxed);
	atomic_store_explicit(__LINE__, &y, 1, memory_order_release);
}

static void c(void *obj)
{
	atomic_load_explicit(__LINE__, &y, memory_order_acquire);
	atomic_store_explicit(__LINE__, &x, 3, memory_order_relaxed);
}

static void b(void *obj)
{
	int r1=atomic_load_explicit(__LINE__, &x, memory_order_relaxed);
	int r2=atomic_load_explicit(__LINE__, &y, memory_order_relaxed);
	MODEL_ASSERT(!(r1 == 3 && r2 == 0));
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2, t3;

	atomic_init(&x, 0);
	atomic_init(&y, 0);

	printf("Main thread: creating 3 threads\n");
	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);
	thrd_create(&t3, (thrd_start_t)&c, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	printf("Main thread is finished\n");

	return 0;
}

/*
	t2: Wx-rel=1; Wx=2; Wy-rel=1;
	t3: r1=Rx; r2=Ry;
	t4: Ry-acq; Wx=3;

	assert(~(r1=3 & r2=0))
*/
