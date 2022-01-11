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
	assert(x == 42);
	return NULL;
}

int main()
{
	thrd_t t1, t2;

	pthread_barrier_init(&barrier, NULL, 2);

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_2, NULL))
		abort();

	/* pthread_barrier_destroy(&barrier); */

	return 0;
}
