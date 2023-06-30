#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <threads.h>

#include "../lamportfm-sc.cc"

int user_main()
{
	thrd_t t[N+1];

	myinit(&x, 0);
	myinit(&y, 0);
	for (intptr_t i = 1; i <= N; i++)
		myinit(&b[i], false);

	for (intptr_t i = N; i >= 1; i--)
		if (thrd_create(&t[i], (thrd_start_t)& thread, NULL))
			MODEL_ASSERT(0);

	for (intptr_t i = 1; i <= N; i++)
		if (thrd_join(t[i]))
			MODEL_ASSERT(0);

	return 0;
}
