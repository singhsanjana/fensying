#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#ifndef N
# define N 2
#endif
#ifndef ROUNDS
# define ROUNDS 2
#endif

pthread_barrier_t barrier;
atomic_int l;

int x; /* test mutual exclusion */

int result[ROUNDS][N];

void *thread_n(void *arg)
{
	intptr_t idx = (intptr_t) arg;

	for (int i = 0u; i < ROUNDS; i++) {

		/* wait for the others */
		pthread_barrier_wait(&barrier);

		/* lock */
		int r = 0;
		while (!atomic_compare_exchange_strong(__FILE__, __LINE__, &l, &r, 1))
			r = 0;

		/* enter cs */
		result[i][idx] = x;
		x++;

		/* unlock */
		l = 0;
	}
	;
}

int user_main()
{
	thrd_t t[N];

	pthread_barrier_init(&barrier, NULL, N);

	for (unsigned i = 0; i < N; i++) {
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);
	}

	for (unsigned i = 0; i < N; i++) {
		if (thrd_join(t[i]))
			MODEL_ASSERT(0);
	}

	/* Test mutual exclusion; all values read should be different in each round */
	for (int i = 0u; i < ROUNDS; i++) {
		for (int j = 0u; j < N; j++)
			for (int k = 0u; k < N; k++)
				if (j != k && result[i][j] == result[i][k])
					MODEL_ASSERT(0);
	}

	/* printf("Alles gut.\n"); */

	pthread_barrier_destroy(&barrier);

	return 0;
}
