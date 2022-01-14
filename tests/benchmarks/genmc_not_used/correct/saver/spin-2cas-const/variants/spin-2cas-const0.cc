#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <genmc.h>

atomic_int x;

void *thread_1(void *unused)
{
	for (int i = 0u; i < 42; i++)
		x = i;
	;
}

void *thread_2(void *unused)
{
	for (;;) {
		int r = 0;
		if (r == 42) {
			if (!atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 42))
				continue;
		} else {
			if (!atomic_compare_exchange_strong(__FILE__, __LINE__, &x, &r, 17))
				continue;
		}
	}

	;
}

int user_user_user_main()
{
	thrd_t t1, t2;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
