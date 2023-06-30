#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex l;

atomic_int x;
int idx[N];

void *thread_n(void *arg)
{
	int new = *((int *) arg);
	int exp = new - 1;

	l.lock();

	if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed) == exp)
		atomic_store_explicit(__FILE__, __LINE__, &x, new, memory_order_relaxed);

	l.unlock();

	;
}
