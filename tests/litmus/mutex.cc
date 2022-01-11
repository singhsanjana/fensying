#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

atomic_int flag1;
atomic_int flag2;
atomic_int x;

static void fn1(void *obj)
{
	atomic_store_explicit(__FILE__, __LINE__, &flag1, 1, memory_order_seq_cst);
    int r = atomic_load_explicit(__FILE__, __LINE__, &flag2, memory_order_relaxed);
    if (r == 0) {
        atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
        int rx = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
        MODEL_ASSERT(rx == 1);
    }
}

static void fn2(void *obj)
{
	atomic_store_explicit(__FILE__, __LINE__, &flag2, 1, memory_order_seq_cst);
    int r = atomic_load_explicit(__FILE__, __LINE__, &flag1, memory_order_relaxed);
    if (r == 0) {
        atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_relaxed);
        int rx = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
        MODEL_ASSERT(rx == 2);
    }
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;

	atomic_init(&flag1, 0);
	atomic_init(&flag2, 0);
	atomic_init(&x, 0);

	thrd_create(&t1, (thrd_start_t)&fn1, NULL);
	thrd_create(&t2, (thrd_start_t)&fn2, NULL);

	thrd_join(t1);
	thrd_join(t2);

	return 0;
}
