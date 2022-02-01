#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../dekker_rlx.cc"

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;
	atomic_init(&x, 0);
	atomic_init(&y, 0);
	atomic_init(&c, 0);

	thrd_create(&t1, (thrd_start_t)& thread_one, NULL);
	// if (thrd_create(&t1, (thrd_start_t)& thread_one, NULL))
	// 	MODEL_ASSERT(0);
	thrd_create(&t2, (thrd_start_t)& thread_two, NULL);
	// if (thrd_create(&t2, (thrd_start_t)& thread_two, NULL))
	// 	MODEL_ASSERT(0);

	return 0;
}
