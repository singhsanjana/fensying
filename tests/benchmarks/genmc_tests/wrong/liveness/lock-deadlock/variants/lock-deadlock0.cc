#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../lock-deadlock.cc"

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;
	atomic_init(&lock_a, 0);
	atomic_init(&lock_b, 0);

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
