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

	for (int i = 0u; i < N; i++)
		thrd_create(&t[i], (thrd_start_t)& thread_n, NULL);

	return 0;
}
