#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../commits.cc"

int user_main()
{
	thrd_t t[THREAD_NUM];

	for (int i = 0; i < THREAD_NUM; i++) {
		idx[i] = i + 1;
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
