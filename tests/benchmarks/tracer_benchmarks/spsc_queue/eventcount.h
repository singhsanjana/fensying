#include <unrelacy.h>
#include <atomic>
#include <mutex>
#include <condition_variable>

#define LOOP 3

class eventcount
{
public:
	eventcount() : waiters(0)
	{
		count.store(__FILE__, __LINE__, 0);
	}

	void signal_relaxed()
	{
		unsigned cmp = count.load(__FILE__, __LINE__, std::memory_order_acquire);
		signal_impl(cmp);
	}

	void signal()
	{
		unsigned cmp = count.fetch_add(__FILE__, __LINE__, 0, std::memory_order_acq_rel);
		signal_impl(cmp);
	}

	unsigned get()
	{
		unsigned cmp = count.fetch_or(__FILE__, __LINE__, 0x80000000, std::memory_order_acq_rel);
		return cmp & 0x7FFFFFFF;
	}

	void wait(unsigned cmp)
	{
		unsigned ec = count.load(__FILE__, __LINE__, std::memory_order_acquire);
		if (cmp == (ec & 0x7FFFFFFF))
		{
			guard.lock($);
			ec = count.load(__FILE__, __LINE__, std::memory_order_acquire);
			if (cmp == (ec & 0x7FFFFFFF))
			{
				waiters += 1;
				cv.wait(guard);
			}
			guard.unlock($);
		}
	}

private:
	std::atomic<unsigned> count;
	rl::var<unsigned> waiters;
	std::mutex guard;
	std::condition_variable cv;

	void signal_impl(unsigned cmp)
	{
		if (cmp & 0x80000000)
		{
        int OK = 0;
        guard.lock($);
        for (int i=0; i<LOOP; i++) { // unroll to do experiment
            if (count.compare_exchange_weak(__FILE__, __LINE__, cmp,
                                            (cmp + 1) & 0x7FFFFFFF,
                                            std::memory_order_acq_rel)) {
                OK = 1;
                break;
            }
        }
  
        if (OK == 1) {
          unsigned w = waiters($);
          waiters = 0;
          guard.unlock($);
          if (w)
              cv.notify_all($);
        }
		}
	}
};
