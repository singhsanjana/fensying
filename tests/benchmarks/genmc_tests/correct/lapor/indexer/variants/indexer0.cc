#include <stdlib.h>
#include <threads.h>#include <stdbool.h>
#include "librace.h" 
#include "model-assert.h"

#ifndef N
# define N 13
#endif

#include "../indexer.c"

int main()
{
	for (int i = 0; i < SIZE; i++)
		pthread_mutex_init(&cas_mutex[i], NULL);

	for (int i = 0; i < N; i++) {
		idx[i] = i;
		pthread_create(&tids[i], NULL,  thread_routine, &idx[i]);
	}

	for (int i = 0; i < N; i++) {
		pthread_join(tids[i], NULL);
	}

	/* for (int i = 0; i < SIZE; i++) { */
	/* 	pthread_mutex_destroy(&cas_mutex[i]); */
	/* } */

}
