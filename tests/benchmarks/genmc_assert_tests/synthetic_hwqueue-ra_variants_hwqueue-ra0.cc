#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include <stdbool.h>
#include "librace.h" 
#include "model-assert.h"

#include "../hwqueue-ra.c"

int user_main()
{
	thrd_t t1, t2, t3, t4;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		abort();
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		abort();
	if (thrd_create(&t3, (thrd_start_t)& thread_3, NULL))
		abort();
	if (thrd_create(&t4, (thrd_start_t)& thread_4, NULL))
		abort();

	if (thrd_join(t1))
		abort();
	if (thrd_join(t2))
		abort();
	if (thrd_join(t3))
		abort();
	if (thrd_join(t4))
		abort();

	MODEL_ASSERT(!(r_1 == 1 && r_2 == 2 && r_3 == 3 && r_4 == 4));

	return 0;
}
