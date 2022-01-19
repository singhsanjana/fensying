#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>

#include "../main.cc"

int user_main()
{
	thrd_t R[MAXREADERS], W[MAXWRITERS], RW[MAXRDWR];


	for (int i = 0; i < readers; i++)
		thrd_create(&R[i], (thrd_start_t)& threadR, NULL);
	for (int i = 0; i < writers; i++)
		thrd_create(&W[i], (thrd_start_t)& threadW, NULL);
	for (int i = 0; i < rdwr; i++)
		thrd_create(&RW[i], (thrd_start_t)& threadRW, NULL);

	for (int i = 0; i < readers; i++)
		pthread_join(R[i], NULL);
	for (int i = 0; i < writers; i++)
		pthread_join(W[i], NULL);
	for (int i = 0; i < rdwr; i++)
		pthread_join(RW[i], NULL);

	return 0;
}
