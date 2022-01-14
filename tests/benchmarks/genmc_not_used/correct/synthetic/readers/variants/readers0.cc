#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../readers.cc"

int user_main()
{
	thrd_t t[N+1];

	for (int i = 0; i <= N; i++)
                atomic_init(&idx[i], i);
	for (int i = 0; i <= N; i++) {
		if (i == 0) {
			if (thrd_create(&t[i], (thrd_start_t)& thread_writer, NULL))
				MODEL_ASSERT(0);
		} else {
			if (thrd_create(&t[i], (thrd_start_t)& thread_reader, NULL))
				MODEL_ASSERT(0);
		}
	}

	for (int i = 0; i <= N; i++) {
		pthread_join(t[i], NULL);
	}

	return 0;
}
