#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../SB+assert.cc"

int user_user_user_main(int argc, char **argv)
{
	thrd_t t0, t1;

	if (thrd_create(&t0, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t1, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(t0))
		MODEL_ASSERT(0);
	if (thrd_join(t1))
		MODEL_ASSERT(0);

	MODEL_ASSERT(!(r_x == 0 && r_y == 0));

	return 0;
}
