#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../2cowr.cc"

int user_user_user_main()
{
	thrd_t t1, t2, t3, t4;

	if (thrd_create(&t3, (thrd_start_t)& thread_three, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t4, (thrd_start_t)& thread_four, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t1, (thrd_start_t)& thread_one, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_two, NULL))
		MODEL_ASSERT(0);

	return 0;
}
