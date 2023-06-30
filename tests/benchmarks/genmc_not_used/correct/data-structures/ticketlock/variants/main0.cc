#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <genmc.h>

#include "../main.cc"

int user_main()
{
	thrd_t t[N];

	ticketlock_init(&lock);
	for (int i = 0u; i < N; i++) {
		if (thrd_create(&t[i], (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);
	}

	return 0;
}
