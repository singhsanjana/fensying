#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdbool.h>

#include "../queue_ok.cc"

int user_user_user_main(void)
{
	thrd_t id1, id2;

	enqueue_flag = true;
	dequeue_flag = false;

	init(&queue);

	if (empty(&queue) != EMPTY)
		__VERIFIER_error();

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
