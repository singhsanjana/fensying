#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int x;

void *thread_1(void *unused)
{
	while (!x.load(__FILE__, __LINE__))
		;
	x = 42;
	;
}

int user_main(int argc, char **argv)
{
	thrd_t t1;
	atomic_init(&x,0);

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);

	return 0;
}
