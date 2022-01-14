#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <genmc.h>

#include "../loop-peel2.cc"

int user_user_user_main()
{
	thrd_t tw, tr[DEFAULT_READERS];

	if (thrd_create(&tw, (thrd_start_t)& thread_writer, NULL))
		MODEL_ASSERT(0);
	for (int i = 0u; i < DEFAULT_READERS; i++) {
		if (thrd_create(&tr[i], (thrd_start_t)& thread_reader, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
