#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

pthread_barrier_t barrier;
atomic_int x;

void *thread_1()
{
	x = 42;
	pthread_barrier_wait(&barrier);
	;
}

void *thread_2()
{
	pthread_barrier_wait(&barrier);
	MODEL_ASSERT(x == 42);
	;
}

int user_main()
{
	thrd_t t1, t2;

	pthread_barrier_init(&barrier, NULL, 2);

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	/* pthread_barrier_destroy(&barrier); */

	return 0;
}
