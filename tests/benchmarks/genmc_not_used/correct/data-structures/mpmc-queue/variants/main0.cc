#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#include "../mpmc-queue.cc"

int user_main()
{
	thrd_t W[MAXWRITERS], R[MAXREADERS], RW[MAXRDWR];

//	printf("%d reader(s), %d writer(s)\n", readers, writers);

#ifndef CONFIG_MPMC_NO_INITIAL_ELEMENT
//	printf("Adding initial element\n");
	int32_t *bin = write_prepare(&queue);
//	store_32(bin, 17);
	*bin = 17;
	write_publish(&queue);
#endif

//	printf("Start threads\n");

	for (int i = 0; i < writers; i++)
		thrd_create(&W[i], (thrd_start_t)& threadW, NULL);
	for (int i = 0; i < readers; i++)
		thrd_create(&R[i], (thrd_start_t)& threadR, NULL);
	for (int i = 0; i < rdwr; i++)
		thrd_create(&RW[i], (thrd_start_t)& threadRW, NULL);

	for (int i = 0; i < writers; i++)
		pthread_join(W[i], NULL);
	for (int i = 0; i < readers; i++)
		pthread_join(R[i], NULL);
	for (int i = 0; i < rdwr; i++)
		pthread_join(RW[i], NULL);

	/* printf("Threads ended\n"); */

	return 0;
}
