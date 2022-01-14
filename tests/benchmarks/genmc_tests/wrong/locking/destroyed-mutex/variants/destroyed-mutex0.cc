#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <genmc.h>

atomic_int x;
atomic_int y;

std::mutex l;

void *thread_1(void *unused)
{
	pthread_mutex_destroy(&l);

	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	;
}

void *thread_2(void *unused)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire))
		if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed))
			l.lock();
	;
}

int user_main()
{
	thrd_t t1, t2;

	pthread_mutex_init(&l, NULL);

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
