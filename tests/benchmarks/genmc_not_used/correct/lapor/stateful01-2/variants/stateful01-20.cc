#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>

#include "../stateful01-2.cc"

int user_user_user_main()
{
	thrd_t  t1, t2;

	pthread_mutex_init(&ma, NULL);
	pthread_mutex_init(&mb, NULL);

	data1 = 10;
	data2 = 10;

	if (thrd_create(&t1, (thrd_start_t)& thread1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread2, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(t1))
		MODEL_ASSERT(0);
	if (thrd_join(t2))
		MODEL_ASSERT(0);

	if (data1 != 16 && data2 != 5)
		__VERIFIER_error();

	return 0;
}
