#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#ifndef N
# define N 2
#endif

pthread_barrier_t barrier;
atomic_int x;

void *thread_n1()
{
	int r = 0;
	while (!atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 1))
		r = 0;

	x = 0;
	;
}

void *thread_n2()
{
	pthread_barrier_wait(&barrier);
	;
}

int user_main()
{
	thrd_t t1[N], t2[N];

	pthread_barrier_init(&barrier, NULL, N);

	for (unsigned i = 0; i < N; i++) {
		if (thrd_create(&t1[i], (thrd_start_t)& thread_n1, NULL))
			MODEL_ASSERT(0);
		if (thrd_create(&t2[i], (thrd_start_t)& thread_n2, NULL))
			MODEL_ASSERT(0);
	}

	/* pthread_barrier_destroy(&barrier); */

	return 0;
}
