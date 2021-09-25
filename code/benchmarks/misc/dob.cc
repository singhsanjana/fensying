#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

atomic_int x;
atomic_int y;

static void a(void *obj)
{
	atomic_store_explicit(__LINE__, &y, 1, memory_order_relaxed);
	atomic_store_explicit(__LINE__, &x, 1, memory_order_release);
	atomic_store_explicit(__LINE__, &x, 2, memory_order_relaxed);
}

static void b(void *obj)
{
	int r1=atomic_load_explicit(__LINE__, &x, memory_order_relaxed);
	int r2=atomic_load_explicit(__LINE__, &y, memory_order_relaxed);
	MODEL_ASSERT(!(r1 == 2 && r2 == 0));
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);

	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);

	thrd_join(t1);
	thrd_join(t2);

	return 0;
}
