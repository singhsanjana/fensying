#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../wcas.cc"

int user_main()
{
	thrd_t t0, t[N];

	if (thrd_create(&t0, (thrd_start_t)& thread_0, NULL))
		MODEL_ASSERT(0);
	for (int i = 0; i < N; i++)
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);

	return 0;
}
