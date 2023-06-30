#include <stdatomic.h>
#include <threads.h>

#define LOOP 5

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
		for (int i=0; i<LOOP; i++) {
			int old_seq = _seq.load(__FILE__, __LINE__, memory_order_acquire); // acquire
			if (old_seq % 2 == 1) continue;

			int res = _data.load(__FILE__, __LINE__, memory_order_acquire); // acquire
			if (_seq.load(__FILE__, __LINE__, memory_order_acquire) == old_seq) { 
				return res;
			}
		}
	}
	
	void write(int new_data) {
		int OK = 0;		
		for (int i=0; i<LOOP; i++) {
			// This might be a relaxed too
			int old_seq = _seq.load(__FILE__, __LINE__, memory_order_acquire); // acquire
			if (old_seq % 2 == 1)
				continue; // Retry

			if (_seq.compare_exchange_strong(__FILE__, __LINE__, old_seq, old_seq + 1,
				memory_order_acq_rel, memory_order_acq_rel)) {
				OK = 1;				
				break;
			}
		}

		if (OK == 1) {
			// Update the data
			_data.store(__FILE__, __LINE__, new_data, memory_order_release); // release
			_seq.fetch_add(__FILE__, __LINE__, 1, memory_order_acq_rel); // release
		}
	}

} seqlock_t;


seqlock_t *lock;

static void a(void *obj) {
	lock->write(3);
}

static void b(void *obj) {
	lock->write(2);
}

static void c(void *obj) {
	int r1 = lock->read();
}

int user_main(int argc, char **argv) {
	thrd_t t1, t2, t3, t4;
	lock = new seqlock_t();

	thrd_create(&t1, (thrd_start_t)&a, NULL);
	thrd_create(&t2, (thrd_start_t)&b, NULL);
	thrd_create(&t3, (thrd_start_t)&c, NULL);

	thrd_join(t1);
	thrd_join(t2);
	thrd_join(t3);
	return 0;
}
