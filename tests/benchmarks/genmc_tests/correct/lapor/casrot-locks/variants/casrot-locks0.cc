#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../casrot-locks.cc"

int user_user_user_main()
{
	thrd_t t[N];

	for (int i = 1; i < N + 1; i++) {
		idx[i - 1] = i;
		if (thrd_create(&t[i - 1], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
