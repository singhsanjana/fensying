#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <threads.h>

#include "../stack-1.cc"

int user_user_user_main(void)
{
	thrd_t id1, id2;

	if (pthread_mutex_init(&m, NULL))
		MODEL_ASSERT(0);

	if (thrd_create(&id1, (thrd_start_t)& t1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&id2, (thrd_start_t)& t2, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(id1))
		MODEL_ASSERT(0);
	if (thrd_join(id2))
		MODEL_ASSERT(0);

	return 0;
}
