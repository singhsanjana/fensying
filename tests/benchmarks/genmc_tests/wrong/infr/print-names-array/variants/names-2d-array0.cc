#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../names-2d-array.cc"

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;
	atomic_init(&x, 0);

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(t1))
		MODEL_ASSERT(0);
	if (thrd_join(t2))
		MODEL_ASSERT(0);

	if (a[1][1] == 42 && stack.nodes[1][1].value == 42)
		MODEL_ASSERT(0);

	return 0;
}
