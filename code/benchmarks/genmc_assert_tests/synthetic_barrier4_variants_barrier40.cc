#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"

pthread_barrier_t barrier;
atomic_int x;

void *thread_1()
{
	x = 42;
	pthread_barrier_wait(&barrier);
	return NULL;
}

void *thread_2()
{
	pthread_barrier_wait(&barrier);
	MODEL_ASSERT(x == 42);
	return NULL;
}

int user_main()
{
	thrd_t t1, t2;

	pthread_barrier_init(&barrier, NULL, 2);

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		abort();
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		abort();

	/* pthread_barrier_destroy(&barrier); */

	return 0;
}
