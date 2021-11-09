/**
 * CDSChecker - peterson
*/

#include <model-assert.h>
#include <threads.h>
#include <atomic>
#include <iostream>

using namespace std;

atomic<int> flag1;
atomic<int> flag2;
atomic<int> turn;
atomic<int> x;

#define LOOP 5

static void t1(void *arg) {
	int ok1 = 0;

	flag1.store(__LINE__, 1, memory_order_seq_cst);
	turn.store(__LINE__, 2, memory_order_seq_cst);

	for (int k = 0; k < LOOP; k++) {
		int f2 = flag2.load(__LINE__, memory_order_acquire);
		int t  = turn.load(__LINE__, memory_order_relaxed);
		if (not(f2 == 1 && t == 2)) {
			ok1 = 1;
			break;
		}
	}
	if (ok1 == 0) return;

	// begin: critical section
	x.store(__LINE__, 1, memory_order_relaxed);
	MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 1);
	// end: critical section

	flag1.store(__LINE__, 0, memory_order_seq_cst);
}

static void t2(void *arg) {
	int ok1 = 0;

	flag2.store(__LINE__, 1, memory_order_seq_cst);
	turn.store(__LINE__, 1, memory_order_seq_cst);

	for (int k = 0; k < LOOP; k++) {
		int f1 = flag1.load(__LINE__, memory_order_acquire);
		int t  = turn.load(__LINE__, memory_order_relaxed);
		if (not(f1 == 1 && t == 1)) {
			ok1 = 1;
			break;
		}
	}
	if (ok1 == 0) return;

	// begin: critical section
	x.store(__LINE__, 2, memory_order_relaxed);
	MODEL_ASSERT(x.load(__LINE__, memory_order_relaxed) == 2);
	// end: critical section

	flag2.store(__LINE__, 0, memory_order_seq_cst);
}

int user_main(int argc, char **argv) {
	thrd_t id1, id2;

	atomic_init(&flag1, 0);
	atomic_init(&flag2, 0);
	atomic_init(&turn, 0);
	atomic_init(&x, 0);

	thrd_create(&id1, (thrd_start_t)&t1, NULL);
	thrd_create(&id2, (thrd_start_t)&t2, NULL);

	thrd_join(id1);
	thrd_join(id2);

	return 0;
}
