#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdatomic.h>
#include <threads.h>

#include "../lockN-r.cc"

int user_user_user_main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++) {
		if (thrd_create(&t[i], (thrd_start_t)& threadn, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
