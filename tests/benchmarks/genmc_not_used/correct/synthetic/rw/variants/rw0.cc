#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../rw.cc"

int user_main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++) {
		idx[i] = i;
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
