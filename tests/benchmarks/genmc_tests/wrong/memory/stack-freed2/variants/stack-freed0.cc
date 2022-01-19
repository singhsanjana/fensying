#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../stack-freed.cc"

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;

	if (thrd_create(&t1, (thrd_start_t)& thread1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
