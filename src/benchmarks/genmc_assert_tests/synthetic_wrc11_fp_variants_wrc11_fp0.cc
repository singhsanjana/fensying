#include <stdio.h>
#include <stdlib.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"
#include <stdatomic.h>

#include "../wrc11_fp.c"

int user_main()
{
	thrd_t t1, t2;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		abort();
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		abort();

	if (thrd_join(t1))
		abort();
	if (thrd_join(t2))
		abort();

	MODEL_ASSERT(!(a == 1 && b == 2));

	return 0;
}
