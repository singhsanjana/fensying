#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex l;

atomic_int x;
atomic_int y;

int r_x;
int r_y;

void *thread_1(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_relaxed);

	l.lock();
	l.unlock();

	r_x = atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed);
	;
}

void *thread_2(void *unused)
{
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);

	l.lock();
	l.unlock();

	r_y = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_relaxed);
	;
}
