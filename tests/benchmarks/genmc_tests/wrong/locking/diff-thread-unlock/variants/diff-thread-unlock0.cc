#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <threads.h>

#include "../diff-thread-unlock.cc"

int user_main()
{
	thrd_t t[N];

        /* pthread_mutex_init(&lock, NULL); */
        lock.lock();
        {
		int i= 0;
		thrd_create(&t[i], (thrd_start_t)& runLock, NULL);
		i++;
		thrd_create(&t[i], (thrd_start_t)& runUnlock, NULL);
		i++;
		MODEL_ASSERT (N == i);
	}
	for (intptr_t i = 0; i < N; i++)
		pthread_join(t[i], NULL);

	return 0;
}
