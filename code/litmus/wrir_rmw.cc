/**
 * misc - eg1-sw
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x ;
atomic<int> y ;
int a,b,c;

static void t1(void *arg) {
	atomic_fetch_add_explicit(__LINE__, &x, 1, memory_order_relaxed);
}

static void t2(void *arg) {
  a = x.load(__LINE__, memory_order_relaxed);
  y.store(__LINE__, 1, memory_order_relaxed);
}

static void t3(void *arg) {
  b = y.load(__LINE__, memory_order_relaxed);
  c = x.load(__LINE__, memory_order_relaxed);
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