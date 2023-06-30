#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../wwr+2wr.cc"

int user_user_user_main()
{
	thrd_t t1, t2, t3;

	if (thrd_create(&t1, (thrd_start_t)& thread1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread2, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t3, (thrd_start_t)& thread3, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(t1))
		MODEL_ASSERT(0);
	if (thrd_join(t2))
		MODEL_ASSERT(0);
	if (thrd_join(t3))
		MODEL_ASSERT(0);

	return 0;
}
