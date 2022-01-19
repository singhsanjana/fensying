#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#ifndef N
# define N 2
#endif

pthread_barrier_t barrier;

void *thread_n()
{
	pthread_barrier_wait(&barrier);
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

	/* pthread_barrier_destroy(&barrier); */

	return 0;
}
