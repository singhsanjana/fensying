#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex l;
atomic_int x;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 42, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	l.lock();
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	l.unlock();
	;
}

void *thread_3(void *unused)
{
	l.lock();
	atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_relaxed);
	atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	l.unlock();
	;
}
