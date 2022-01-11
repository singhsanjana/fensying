#include <stdlib.h>
#include <stdio.h>
#include <threads.h>#include <stdbool.h>
#include "librace.h" 
#include "model-assert.h"
#include <stdatomic.h>

#include "../main.c"

int user_main()
{
	int i = 0;
	unsigned int in_sum = 0, out_sum = 0;

	queue = &myqueue;
	num_threads = readers + writers + rdwr;

	init_queue(queue, num_threads);
	for (int j = 0; j < num_threads; j++)
		param[j] = j;
	for (int j = 0; j < writers; j++, i++)
		thrd_create(&threads[i], (thrd_start_t)& threadW, NULL);
	for (int j = 0; j < readers; j++, i++)
		thrd_create(&threads[i], (thrd_start_t)& threadR, NULL);
	for (int j = 0; j < rdwr; j++, i++)
		thrd_create(&threads[i], (thrd_start_t)& threadRW, NULL);

	/* for (i = 0; i < num_threads; i++) { */
	/* 	param[i] = i; */
	/* 	thrd_create(&threads[i], (thrd_start_t)& main_task, NULL); */
	/* } */

	/* for (i = 0; i < num_threads; i++) { */
	/* 	in_sum += input[i]; */
	/* 	out_sum += output[i]; */
	/* } */
	/* for (i = 0; i < num_threads; i++) */
	/* 	printf("input[%d] = %u\n", i, input[i]); */
	/* for (i = 0; i < num_threads; i++) */
	/* 	printf("output[%d] = %u\n", i, output[i]); */
	/* if (succ1 && succ2) */
	/* 	MODEL_ASSERT(in_sum == out_sum); */
	/* else */
	/* 	MODEL_ASSERT(0); */

	return 0;
}
