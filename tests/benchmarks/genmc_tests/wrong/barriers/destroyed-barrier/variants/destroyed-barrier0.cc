#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int x;
atomic_int y;

pthread_barrier_t barrier;

void *thread_1(void *unused)
{
	pthread_barrier_wait(&barrier);
	pthread_barrier_destroy(&barrier);

	atomic_store_explicit(__FILE__, __LINE__, &x, 1, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &y, 1, memory_order_release);
	;
}

void *thread_2(void *unused)
{
	if (atomic_load_explicit(__FILE__, __LINE__, &y, memory_order_acquire))
		if (atomic_load_explicit(__FILE__, __LINE__, &x, memory_order_relaxed))
			pthread_barrier_wait(&barrier);
	;
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;

	pthread_barrier_init(&barrier, NULL, 1);

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
