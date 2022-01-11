/**
 * misc - eg1-sw
*/
#include "hb2.cc"

static void t3(void *arg) {
  c = y.load(__FILE__, __LINE__, memory_order_relaxed);
  z.store(__FILE__, __LINE__, 1, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
    thrd_t th1, th2, th3;

    atomic_init(&x, 0);
    atomic_init(&y, 0);
    atomic_init(&z, 0);
    a = b = c = 0;

    thrd_create(&th1, (thrd_start_t)&t1, NULL);
    thrd_create(&th2, (thrd_start_t)&t2, NULL);
    thrd_create(&th3, (thrd_start_t)&t3, NULL);

    thrd_join(th1);
    thrd_join(th2);
    thrd_join(th3);

    MODEL_ASSERT(!(a == 1 && b == 1 && c == 1));

    return 0;
}