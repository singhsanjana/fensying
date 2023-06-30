#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <genmc.h>

#include "../loop-peel.cc"

int user_user_user_main()
{
	thrd_t tw[DEFAULT_WRITERS], tr;

	if (thrd_create(&tr, (thrd_start_t)& thread_reader, NULL))
		MODEL_ASSERT(0);
	for (int i = 0u; i < DEFAULT_WRITERS; i++)
		if (thrd_create(&tw[i], (thrd_start_t)& thread_writer, NULL))
			MODEL_ASSERT(0);


	return 0;
}
