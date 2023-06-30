#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../wrc11_fp.cc"

int user_main()
{
	thrd_t t1, t2;

	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(t2))
		MODEL_ASSERT(0);
	if (thrd_join(t1))
		MODEL_ASSERT(0);

	MODEL_ASSERT(!(a == 1 && b == 2));

	return 0;
}
