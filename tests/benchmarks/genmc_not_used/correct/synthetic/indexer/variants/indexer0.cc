#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdbool.h>
#include <stdatomic.h>

#include "../indexer.cc"

int idx[N];

int user_main()
{
	thrd_t t[N];

	for (int i = 0; i < N; i++) {
		idx[i] = i;
		thrd_create(&t[i], (thrd_start_t)& thread_n, NULL);
	}

	return 0;
}
