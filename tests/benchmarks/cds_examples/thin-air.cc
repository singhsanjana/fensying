/**
 * CDSChecker - thin-air
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;
#define LOOP 1

atomic<int> x;
atomic<int> y;
int a,b;

static void fn1(void *obj) {
	a=x.load(__LINE__, memory_order_relaxed);
	for (int i = 0; i < LOOP; i++) {
		y.store(__LINE__, 1, memory_order_relaxed);
	}
}

static void fn2(void *obj) {
	b=y.load(__LINE__, memory_order_relaxed);
	for (int i = 0; i < LOOP; i++) {
		x.store(__LINE__, 1, memory_order_relaxed);
	}
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2;

	atomic_init(&x, 0);
	atomic_init(&y, 0);
	a = b = 0;

	thrd_create(&t1, (thrd_start_t)&fn1, NULL);
	thrd_create(&t2, (thrd_start_t)&fn2, NULL);

	thrd_join(t1);
	thrd_join(t2);

	MODEL_ASSERT(!(a == 1 && b == 1));

	return 0;
}
