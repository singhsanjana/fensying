#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int x;

void *thread_n(void *unused)
{
	int a = x;
	x = 1;
	int b = x;
	;
}

int user_user_user_main()
{
	thrd_t t1, t2;

	if (thrd_create(&t1, (thrd_start_t)& thread_n, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_n, NULL))
		MODEL_ASSERT(0);

	return 0;
}
