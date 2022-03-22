#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"

int a;
atomic<int> b;

static void fn1(void *obj)
{
	store_32(&a,1);
    b.store(__FILE__, __LINE__, 1, memory_order_release);
}

static void fn2(void *obj)
{
    int k = b.load(__FILE__, __LINE__, memory_order_acquire);
	int l = load_32(&a);
    MODEL_ASSERT(k == 0 || l != 0);
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;

	a = 0;
    atomic_init(&b, 0);

	thrd_create(&t1, (thrd_start_t)&fn1, NULL);
	thrd_create(&t2, (thrd_start_t)&fn2, NULL);

	thrd_join(t1);
	thrd_join(t2);
    
	return 0;
}
