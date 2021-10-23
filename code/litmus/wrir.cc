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
atomic<int> a ;
atomic<int> b ;
atomic<int> c ;

static void t1(void *arg) {
	x.store(__LINE__, 1, memory_order_relaxed);
}

static void t2(void *arg) {
  int x_temp = x.load(__LINE__, memory_order_relaxed);
  a.store(__LINE__, x_temp, memory_order_relaxed);
  y.store(__LINE__, 1, memory_order_relaxed);
}

static void t3(void *arg) {
  int y_temp = y.load(__LINE__, memory_order_relaxed);
  b.store(__LINE__, y_temp, memory_order_relaxed);
  int x_temp = x.load(__LINE__, memory_order_relaxed);
  c.store(__LINE__, x_temp, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t th1, th2, th3;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&a, 0);
    atomic_init(&b, 0);
    atomic_init(&c, 0);

    thrd_create(&th1, (thrd_start_t)&t1, NULL);
    thrd_create(&th2, (thrd_start_t)&t2, NULL);
    thrd_create(&th3, (thrd_start_t)&t3, NULL);

    thrd_join(th1); // then join more
    thrd_join(th2);
    thrd_join(th3);

    int a_temp = a.load(__LINE__, memory_order_relaxed);
    int b_temp = b.load(__LINE__, memory_order_relaxed);
    int c_temp = c.load(__LINE__, memory_order_relaxed);
    MODEL_ASSERT(!(a_temp == 1 && b_temp == 1 && c_temp == 0));

    return 0;
}