#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdbool.h>

#ifndef N
# define N 13
#endif

#include "../indexer.cc"

int user_user_user_main()
{
	for (int i = 0; i < SIZE; i++)
		pthread_mutex_init(&cas_mutex[i], NULL);

	for (int i = 0; i < N; i++) {
		idx[i] = i;
		thrd_create(&tids[i], (thrd_start_t)&  thread_routine, NULL);
	}

	for (int i = 0; i < N; i++) {
		pthread_join(tids[i], NULL);
	}

	/* for (int i = 0; i < SIZE; i++) { */
	/* 	pthread_mutex_destroy(&cas_mutex[i]); */
	/* } */

}
