#include <stdio.h>
#include <threads.h>

#include "mcs_lock.h"

/* For data race instrumentation */
#include "librace.h"

struct mcs_mutex *mutex;

void threadA(void *arg)
{
	mcs_mutex::guard g(mutex);
	if (mutex->unlock(&g))
		mutex->lock(&g);
}

void threadB(void *arg)
{
	mcs_mutex::guard g(mutex);
	if (mutex->unlock(&g))
		mutex->lock(&g);
}

int user_main(int argc, char **argv)
{
	thrd_t A, B;

	mutex = new mcs_mutex();

	thrd_create(&A, &threadA, NULL);
	thrd_create(&B, &threadB, NULL);
	thrd_join(A);
	thrd_join(B);
	return 0;
}
