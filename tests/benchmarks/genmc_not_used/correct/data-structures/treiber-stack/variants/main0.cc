#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdbool.h>
#include <threads.h>

#include "../main.cc"

int user_main()
{
	int i = 0;

	num_threads = readers + writers + rdwr;

	atomic_init(&x[1], 0);
	atomic_init(&x[2], 0);

	init_stack(&stack, num_threads);
	for (int j = 0; j < num_threads; j++)
		param[j] = j;
	for (int j = 0; j < writers; j++, i++)
		thrd_create(&threads[i], (thrd_start_t)& threadW, NULL);
	for (int j = 0; j < readers; j++, i++)
		thrd_create(&threads[i], (thrd_start_t)& threadR, NULL);
	for (int j = 0; j < rdwr; j++, i++)
		thrd_create(&threads[i], (thrd_start_t)& threadRW, NULL);

	for (i = 0; i < num_threads; i++)
		pthread_join(threads[i], NULL);

	/* bool correct = false; */
	//correct |= (a == 17 || a == 37 || a == 0);
	//MODEL_ASSERT(correct);

	/* free(param); */
	/* free(threads); */
	/* free(stack); */

	return 0;
}
