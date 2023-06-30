#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdbool.h>
#include <threads.h>

#include "../main.cc"

int user_main()
{
	thrd_t W, R[NUMREADERS];

	if (thrd_create(&W, (thrd_start_t)& threadW, NULL))
		MODEL_ASSERT(0);
	for (int i = 0; i < NUMREADERS; i++)
		if (thrd_create(&R[i], (thrd_start_t)& threadR, NULL))
			MODEL_ASSERT(0);

	for (int i = 0; i < NUMREADERS; i++)
		if (thrd_join(R[i]))
			MODEL_ASSERT(0);
	if (thrd_join(W))
		MODEL_ASSERT(0);

	return 0;
}
