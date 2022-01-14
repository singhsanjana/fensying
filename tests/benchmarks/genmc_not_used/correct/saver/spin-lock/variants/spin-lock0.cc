#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <genmc.h>

#ifndef THREADS
# define THREADS 2
#endif

#ifndef MAX
# define MAX 4
#endif

#ifndef DEF
# define DEF (-42)
#endif

std::mutex l;
int a[MAX] = { [0 ... MAX-1] = DEF };

int check_array(int array[MAX])
{
	for (int i = 0u; i < MAX; i++) {
		if (a[i] == DEF)
			return 0;
	}
	return 1;
}

void *thread_checker(void *unused)
{
	while (1) {
		/* Check whether all threads have finished */
		l.lock();
		if (check_array(a)) {
			l.unlock();
			break;
		}
		l.unlock();
	}
	;
}

void *thread_worker(void *arg)
{
	intptr_t index = (intptr_t) arg;

	for (intptr_t i = index * MAX / THREADS; i < MAX && i < (index + 1) * MAX / THREADS; ++i) {
		/* Do some work */
		int result = i;

		/* Write result to its place */
		l.lock();
		a[i] = result;
		l.unlock();
	}
	;
}

int user_user_user_main()
{
	thrd_t tc, tw[THREADS];

	if (thrd_create(&tc, (thrd_start_t)& thread_checker, NULL))
		MODEL_ASSERT(0);
	for (int i = 0u; i < THREADS; i++) {
		if (thrd_create(&tw[i], (thrd_start_t)& thread_worker, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
