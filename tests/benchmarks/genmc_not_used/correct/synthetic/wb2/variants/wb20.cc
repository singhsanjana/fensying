#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../wb2.cc"

int user_main()
{
	thrd_t t1, t2, t3, t4;

	if (thrd_create(&t1, (thrd_start_t)& thread1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread2, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t3, (thrd_start_t)& thread3, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t4, (thrd_start_t)& thread4, NULL))
		MODEL_ASSERT(0);

	return 0;
}
