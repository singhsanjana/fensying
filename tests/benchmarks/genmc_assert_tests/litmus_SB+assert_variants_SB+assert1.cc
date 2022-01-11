#include <stdlib.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"
#include <stdatomic.h>

#include "../SB+MODEL_ASSERT.c"

int user_main(int argc, char **argv)
{
	thrd_t t0, t1;

	if (thrd_create(&t0, (thrd_start_t)& thread_1, NULL))
		abort();
	if (thrd_create(&t1, (thrd_start_t)& thread_2, NULL))
		abort();

	if (thrd_join(t1))
		abort();
	if (thrd_join(t0))
		abort();

	MODEL_ASSERT(!(r_x == 0 && r_y == 0));

	return 0;
}
