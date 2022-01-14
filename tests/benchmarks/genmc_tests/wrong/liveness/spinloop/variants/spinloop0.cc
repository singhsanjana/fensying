#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int x;

void *thread_1(void *unused)
{
	while (!x)
		;
	x = 42;
	;
}

int user_main()
{
	thrd_t t1;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);

	return 0;
}
