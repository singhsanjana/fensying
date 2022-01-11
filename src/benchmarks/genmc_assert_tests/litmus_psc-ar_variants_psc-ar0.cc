#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"

#include "../psc-ar.c"

int user_main()
{
	thrd_t t1, t2, t3;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		abort();
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		abort();
	if (thrd_create(&t3, (thrd_start_t)& thread_3, NULL))
		abort();

	if (thrd_join(t1))
		abort();
	if (thrd_join(t2))
		abort();
	if (thrd_join(t3))
		abort();

#ifdef CHECK_ASSERTION
	MODEL_ASSERT(!(res == 1));
#endif
	return 0;
}
