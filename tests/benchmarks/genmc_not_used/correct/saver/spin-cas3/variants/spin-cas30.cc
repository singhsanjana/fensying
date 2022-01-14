#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#ifndef N
# define N 3
#endif

#include "../spin-cas3.cc"

int user_user_user_main()
{
	thrd_t t[N];

	for (unsigned i = 0; i < N; i++) {
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
