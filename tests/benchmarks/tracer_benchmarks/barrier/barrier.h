#include <atomic>
#include <stdatomic.h>

#define LOOP 10

class spinning_barrier {
 public:
	spinning_barrier (unsigned int n) : n_ (n) {
		nwait_ = 0;
		step_ = 0;
	}

	bool wait() {
		unsigned int step = step_.load (__FILE__, __LINE__, memory_order_acquire);

		if (nwait_.fetch_add (__FILE__, __LINE__, 1, memory_order_acq_rel) == n_ - 1) {
			/* OK, last thread to come.  */
			nwait_.store (__FILE__, __LINE__, 0, memory_order_release); // XXX: maybe can use relaxed ordering here ??
			step_.fetch_add (__FILE__, __LINE__, 1, memory_order_acq_rel);
			return true;
		} else {
			/* Run in circles and scream like a little girl.  */
			//while (step_.load (__FILE__, __LINE__, memory_order_acquire) == step)
			//	thrd_yield();
			for (int i=0; i<LOOP; i++) 
			{
				if (step_.load (__FILE__, __LINE__, memory_order_acquire) != step)
					break;
			}
		
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
