#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <threads.h>

#include "model-assert.h"

using namespace std;

atomic<int> x;
atomic<int> y;

static void fn1(void*) {
    x.store(__FILE__, __LINE__, 17, memory_order_relaxed);
    int k = 0;
    y.compare_exchange_strong(__FILE__, __LINE__, k, 1, memory_order_relaxed);
    return;
}

static void fn2(void*) {
    int k = 1;
    if ( y.compare_exchange_strong(__FILE__, __LINE__, k, 0, memory_order_relaxed) ) {
        int l = x.load(__FILE__, __LINE__, memory_order_relaxed);
        MODEL_ASSERT( l==17 );
    }
    return;
}

int user_main(int argc, char **argv) {
    atomic_init(&x, 0);
    atomic_init(&y, 0);

    thrd_t t1, t2;
    thrd_create(&t1, fn1, NULL);
    thrd_create(&t2, fn2, NULL);

    thrd_join(t1);
    thrd_join(t2);

    return 0;
}