#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../MCP-rrw.cc"

int user_user_user_main()
{
	thrd_t t1, t2, t3;

	if (thrd_create(&t3, (thrd_start_t)& thread_3, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
