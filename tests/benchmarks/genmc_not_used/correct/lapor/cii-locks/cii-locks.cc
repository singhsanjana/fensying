#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex l;

atomic_int x;

void *thread_1(void *unused)
{
	int r = 1;

	l.lock();
	if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed) == r)
		atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_relaxed);
	l.unlock();

	;
}

void *thread_2(void *unused)
{
	l.lock();
	int r = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, r + 1, memory_order_relaxed);
	l.unlock();
	;
}

void *thread_3(void *unused)
{
	l.lock();
	int r = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &x, r + 1, memory_order_relaxed);
	l.unlock();
	;
}
