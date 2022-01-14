#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>

#include "../lockinc.cc"

int user_main()
{
	thrd_t t[N];

	myinit(&l, 0);

	for (int i = 0; i < N; i++)
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);

	for (int i = 0; i < N; i++)
		if (thrd_join(t[i]))
			MODEL_ASSERT(0);

	return 0;
}
