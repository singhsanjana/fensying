#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../N_writers_a_reader.cc"

int user_main()
{
	thrd_t tr, tw[N + 1];

	if (thrd_create(&tr, (thrd_start_t)& threadR, NULL))
		MODEL_ASSERT(0);
	for (int i = 0; i <= N; i++) {
		idx[i] = i;
		if (thrd_create(&tw[i], (thrd_start_t)& threadW, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
