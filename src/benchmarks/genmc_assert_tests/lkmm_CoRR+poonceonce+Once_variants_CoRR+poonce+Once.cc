#include <stdlib.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"
#include <lkmm.h>

int x;

int r0, r1;

void *thread_1(void *unused)
{
	WRITE_ONCE(x, 1);
	return NULL;
}

void *thread_2(void *unused)
{
	r0 = READ_ONCE(x);
	r1 = READ_ONCE(x);
	return NULL;
}

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

	MODEL_ASSERT(!(r0 == 1 && r1 == 0));

	return 0;
}
