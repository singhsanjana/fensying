#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../inc+inc+rr+w+rr.cc"

int user_user_user_main()
{
	thrd_t t1, t2, t3, t4, t5;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t3, (thrd_start_t)& thread_3, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t4, (thrd_start_t)& thread_4, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t5, (thrd_start_t)& thread_5, NULL))
		MODEL_ASSERT(0);


	if (thrd_join(t1))
		MODEL_ASSERT(0);
	if (thrd_join(t2))
		MODEL_ASSERT(0);
	if (thrd_join(t3))
		MODEL_ASSERT(0);
	if (thrd_join(t4))
		MODEL_ASSERT(0);
	if (thrd_join(t5))
		MODEL_ASSERT(0);

	MODEL_ASSERT (!(a == 42 && b == 2 && c == 1 && d == 42));

	return 0;
}
