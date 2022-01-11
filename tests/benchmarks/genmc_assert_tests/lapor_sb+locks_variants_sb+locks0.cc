#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"

#include "../sb+locks.c"

int user_main()
{
	thrd_t t1, t2;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		abort();
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		abort();

	if (thrd_join(t1))
		abort();
	if (thrd_join(t2))
		abort();

	MODEL_ASSERT(!(r_x == 0 && r_y == 0));

	return 0;
}
