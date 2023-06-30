#include <stdatomic.h>
#include <threads.h>

#include "librace.h"
#include "model-assert.h"

#define RLOOP 1
#define WLOOP 2

#define Nwriters 2

#define default_ret -42

atomic_int sanity_check;

typedef struct seqlock {
	// Sequence for reader consistency check
	atomic_int _seq;
	// It needs to be atomic to avoid data races
	atomic_int _data;

	seqlock() {
		atomic_init(&_seq, 0);
		atomic_init(&_data, 0);
	}

	int read() {
		// while (true) {
		for (int i = 0 ; i < RLOOP; i++) {
			int old_seq = _seq.load(__FILE__, __LINE__, memory_order_seq_cst); // acquire
			if (old_seq % 2 == 1) continue;

			int res = _data.load(__FILE__, __LINE__, memory_order_seq_cst); // acquire
			if (_seq.load(__FILE__, __LINE__, memory_order_seq_cst) == old_seq) { // relaxed
				return res;
			}
		}

		return default_ret;
	}
	
	void write(int new_data) {
		bool locked = false;

		// while (true) {
		for (int i = 0 ; i < WLOOP; i++) {
			// This might be a relaxed too
			int old_seq = _seq.load(__FILE__, __LINE__, memory_order_seq_cst); // acquire
			if (old_seq % 2 == 1) {
				continue; // Retry
			}

			// Should be relaxed!!! 
			if (_seq.compare_exchange_strong(__FILE__, __LINE__, old_seq, old_seq + 1, memory_order_relaxed)) {
				locked = true;
				break;
			}
		}

		if (!locked) return;

		sanity_check.store(__FILE__, __LINE__, new_data, memory_order_relaxed);
		int sc = sanity_check.load(__FILE__, __LINE__, memory_order_relaxed);
		MODEL_ASSERT(sc == new_data);

		// Update the data
		_data.store(__FILE__, __LINE__, new_data, memory_order_seq_cst); // release

		_seq.fetch_add(__FILE__, __LINE__, 1, memory_order_relaxed); // release
	}

} seqlock_t;


seqlock_t *lock;

static void a(void *obj) {
	lock->write(3);
}

static void b(void *obj) {
	int val = *(static_cast<int*>(obj));
	lock->write(val);
}

static void c(void *obj) {
	int r1 = lock->read();
}

int user_main(int argc, char **argv) {
	atomic_init(&sanity_check, 0);

	thrd_t tR, tW[Nwriters];
	lock = new seqlock_t();

	thrd_create(&tR, (thrd_start_t)&c, NULL);
	for (int w = 0; w < Nwriters; w++) {
		int *k = new int(w+1);
		thrd_create(&tW[w], (thrd_start_t)&b, k);
	}
	
	thrd_join(tR);
	for (int w = 0; w < Nwriters; w++) {
		thrd_join(tW[w]);
	}
	return 0;
}
