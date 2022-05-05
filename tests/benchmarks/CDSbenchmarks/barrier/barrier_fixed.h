#include <atomic>

#define LOOP 10

class spinning_barrier {
 public:
	spinning_barrier (unsigned int n) : n_ (n) {
		nwait_.store(__FILE__, __LINE__, 0, std::memory_order_seq_cst);
		step_.store(__FILE__, __LINE__, 0, std::memory_order_seq_cst);
	}

	bool wait() {
atomic_thread_fence(__FILE__, __LINE__, std::memory_order_seq_cst);
		unsigned int step = step_.load (__FILE__, __LINE__, std::memory_order_seq_cst);

		if (nwait_.fetch_add (__FILE__, __LINE__, 1, std::memory_order_seq_cst) == n_ - 1) {
			/* OK, last thread to come.  */
			nwait_.store (__FILE__, __LINE__, 0, std::memory_order_seq_cst);
atomic_thread_fence(__FILE__, __LINE__, std::memory_order_seq_cst);
			step_.fetch_add (__FILE__, __LINE__, 1, std::memory_order_relaxed);
			return true;
		} else {
			/* Run in circles and scream.  */
			for (int i= 0; i < LOOP; i++) {
				if (step_.load (__FILE__, __LINE__, std::memory_order_seq_cst) == step) {
				}
				else
					return true;
			}
			// while (step_.load (__FILE__, __LINE__, std::memory_order_seq_cst) == step && i < LOOP) {
			// 	i++;
			// 	// thrd_yield();
			// }
			return false;
		}
	}

 protected:
	/* Number of synchronized threads. */
	const unsigned int n_;

	/* Number of threads currently spinning.  */
	std::atomic<unsigned int> nwait_;

	/* Number of barrier syncronizations completed so far, 
	 *      * it's OK to wrap.  */
	std::atomic<unsigned int> step_;
};
