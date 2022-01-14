#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <threads.h>
#include <genmc.h>

#include "../main.cc"

int user_main(int argc, char **argv)
{
	struct combiner cmb;

	/* initialize the set and the combiner */
	init();
	init_combiner(&cmb);

	for (int i = 1; i < num_threads; i++)
		thrd_create(&threads[i], (thrd_start_t)& thread_n, NULL);
	for (int i = 1; i < num_threads; i++)
		pthread_join(threads[i], NULL);

	return 0;
}
