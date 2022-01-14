#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdatomic.h>
#include <threads.h>

#include "../chase-lev.cc"

int user_main()
{
	thrd_t t0, ts[DEFAULT_STEALERS];

	if (thrd_create(&t0, (thrd_start_t)& thread_pp, NULL))
		MODEL_ASSERT(0);
	for (int i = 0; i < DEFAULT_STEALERS; i++) {
		if (thrd_create(&ts[i], (thrd_start_t)& thread_s, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
