#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdatomic.h>
#include <threads.h>

#include "../lockN-ww.cc"

int user_user_user_main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++)
		idx[i] = i;

	for (int i = 0; i < N; i++) {
		if (thrd_create(&t[i], (thrd_start_t)& threadn, NULL))
			MODEL_ASSERT(0);
	}
	for (int i = 0; i < N; i++) {
		if (thrd_join(t[i]))
			MODEL_ASSERT(0);
	}

	MODEL_ASSERT (x == y);

	return 0;
}
