#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>

#include "../corr0-sc.cc"

int user_main()
{
	thrd_t t1, t2, t3;

	myinit(&x, 0);

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_3, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t3, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(t1))
		MODEL_ASSERT(0);
	if (thrd_join(t3))
		MODEL_ASSERT(0);
	if (thrd_join(t2))
		MODEL_ASSERT(0);

	return 0;
}
