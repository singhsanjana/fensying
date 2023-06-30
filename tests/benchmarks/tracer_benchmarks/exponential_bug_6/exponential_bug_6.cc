/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Example in Figure 2 in the PLDI 2015 paper: 
   https://dl.acm.org/citation.cfm?id=2737975
*/

#include <threads.h>
#include <stdatomic.h>
#include <mutex>
#include "librace.h"
#include "model-assert.h"

#define N 6

// shared variables
atomic_int x, y;
std::mutex *lock;

void thr1(void *arg)
{
	lock->lock();
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	for (int i=0; i<N; i++) {
		atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	}
	lock->unlock();

	lock->lock();
	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_release);
	for (int i=0; i<N; i++) {
		atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	}
	lock->unlock();

}

void thr2(void *arg)
{
	int _y;

	lock->lock();
	atomic_store_explicit(__FILE__, __LINE__, &x, 0, memory_order_release);
	lock->unlock();
	
	if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire) > 0) {
		_y = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
		atomic_store_explicit(__FILE__, __LINE__, &y, _y+1, memory_order_release);
		atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
	}

	lock->lock();
	atomic_store_explicit(__FILE__, __LINE__, &x, 0, memory_order_release);
	lock->unlock();
	
	if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire) > 0) {
		_y = atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire);
		atomic_store_explicit(__FILE__, __LINE__, &y, _y+1, memory_order_release);
		atomic_store_explicit(__FILE__, __LINE__, &x, 2, memory_order_release);
	}
}


void thr3(void *arg)
{
	
	if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire) > 1) {
		if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire) == 3) {
			//MODEL_ASSERT(0);
			return;
		} else {
			atomic_store_explicit(__FILE__, __LINE__, &y, 2, memory_order_release);
		}
	}

	if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_acquire) > 1) {
		if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire) == 3) {
			//MODEL_ASSERT(0);
			return;
		} else {
			atomic_store_explicit(__FILE__, __LINE__, &y, 2, memory_order_release);
		}
	}
}

int user_main(int argc, char **argv)
{

	thrd_t t1, t2, t3;
	
	atomic_init(&x, 0);
	atomic_init(&y, 0);
	
	lock = new std::mutex();

	thrd_create(&t1, thr1, NULL);
	thrd_create(&t2, thr2, NULL);
	thrd_create(&t3, thr3, NULL);

	return 0;
}
