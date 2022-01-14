#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdbool.h>

#include "../main.cc"

int user_user_user_main()
{
	num_threads = N;
	for (int j = 0; j < num_threads; j++)
		param[j] = j;

	add(&myheap, 0, 0);

	for (int i = 0; i < num_threads; i++)
		if (thrd_create(&threads[i + 1], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);

	for (int i = 0; i < num_threads; i++)
		if (thrd_join(threads[i + 1]))
			MODEL_ASSERT(0);

	/* for (int i = 0; i < num_threads; i++) { */
	/* 	if (thrd_create(&threads[i], (thrd_start_t)& thread_n, NULL)) */
	/* 		MODEL_ASSERT(0); */
	/* } */

	/* for (int i = 0; i < num_threads; i++) { */
	/* 	if (thrd_join(threads[i])) */
	/* 		MODEL_ASSERT(0); */
	/* } */

	/* for (int i = 0; i < myheap.next; i++) */
	/* 	printf("%d ", myheap.nodes[i].score); */
	/* printf("success\n"); */

	return 0;
}
