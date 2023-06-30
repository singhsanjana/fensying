#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>

#include "../lastzero-sc.cc"

int user_main()
{
	thrd_t t[N+1];

	for (int i = 0; i <=N; i++)
		myinit(&array[i], 0);

	for (int i = 0; i <= N; i++) {
		idx[i] = i;
		if (i == 0) {
			if (thrd_create(&t[i], (thrd_start_t)& thread_reader, NULL))
				MODEL_ASSERT(0);
		} else {
			if (thrd_create(&t[i], (thrd_start_t)& thread_writer, NULL))
				MODEL_ASSERT(0);
		}
	}

	return 0;
}
