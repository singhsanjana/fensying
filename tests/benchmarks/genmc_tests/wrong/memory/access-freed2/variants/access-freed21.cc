#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../access-freed2.cc"

int user_main()
{
	thrd_t t1, t2;

	p = malloc(sizeof(atomic_int));
	atomic_init(p, 0);

	if (thrd_create(&t2, (thrd_start_t)& thread2, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t1, (thrd_start_t)& thread1, NULL))
		MODEL_ASSERT(0);

	return 0;
}
