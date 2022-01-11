#include <stdlib.h>
#include <stdio.h>
#include <threads.h>#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"

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
		while (!atomic_compare_exchange_strong(&l, &r, 1))
			r = 0;

		/* enter cs */
		result[i][idx] = x;
		x++;

		/* unlock */
		l = 0;
	}
	return NULL;
}

int user_main()
{
	thrd_t t[N];

	pthread_barrier_init(&barrier, NULL, N);

	for (unsigned i = 0; i < N; i++) {
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			abort();
	}

	for (unsigned i = 0; i < N; i++) {
		if (thrd_join(t[i]))
			abort();
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
