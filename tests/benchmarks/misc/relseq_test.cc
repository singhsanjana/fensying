/**
 * misc - basic
*/

#include <iostream>
#include <threads.h>
#include <atomic>
#include <model-assert.h>

using namespace std;

atomic<int> x;

static void t1(void *arg) {
	x.store(__LINE__, 1, memory_order_release);
    x.store(__LINE__, 2, memory_order_release);
    x.store(__LINE__, 3, memory_order_relaxed);
}

static void t2(void *arg) {
	x.load(__LINE__, memory_order_relaxed);
    x.store(__LINE__, 4, memory_order_release);
    x.store(__LINE__, 5, memory_order_relaxed);
}

int user_main(int argc, char **argv) {
	thrd_t id1, id2;

    atomic_init(&x, 0);

	thrd_create(&id1, (thrd_start_t)&t1, NULL);
    thrd_create(&id2, (thrd_start_t)&t2, NULL);

    thrd_join(id1);
    thrd_join(id2);

    MODEL_ASSERT (x.load(__LINE__, memory_order_relaxed) == 0);

	return 0;
}
