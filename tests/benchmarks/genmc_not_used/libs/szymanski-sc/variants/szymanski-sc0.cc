#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>

#include "../szymanski-sc.cc"

int user_main()
{
	thrd_t t1, t2;

	myinit(&x, 0);
	myinit(&flag1, 0);
	myinit(&flag2, 0);

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(t1))
		MODEL_ASSERT(0);
	if (thrd_join(t2))
		MODEL_ASSERT(0);

	return 0;
}
