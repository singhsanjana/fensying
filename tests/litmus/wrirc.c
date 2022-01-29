/**
 * misc - eg1-sw
*/

#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>
#include <model-assert.h>

atomic_int x ;
atomic_int y ;
int a,b,c;

static void t1(void *arg) {
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
}

static void t2(void *arg) {
  a = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
  atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);
}

static void t3(void *arg) {
  b = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
  c = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t th1, th2, th3;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    a = b = c = 0;

    thrd_create(&th1, (thrd_start_t)&t1, NULL);
    thrd_create(&th2, (thrd_start_t)&t2, NULL);
    thrd_create(&th3, (thrd_start_t)&t3, NULL);

    thrd_join(th1);
    thrd_join(th2);
    thrd_join(th3);

    MODEL_ASSERT(!(a == 1 && b == 1 && c == 0));

    return 0;
}