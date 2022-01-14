#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex mutex;
atomic_int x;

void *thread_n(void *unused)
{
	int r;

	mutex.lock();
	r = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, r + 1, memory_order_relaxed);
	mutex.unlock();
	;
}
