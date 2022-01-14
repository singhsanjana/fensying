#include "librace.h" 
#include "model-assert.h"
#include <stdint.h>
#include <stdatomic.h>
#include <threads.h>

#include "../par_length.cc"

int user_main(int argc, char **argv)
{
	thrd_t tid[T];

	for (int i = 0; i < T; ++i) {
		thrd_create(tid + i, (thrd_start_t)& thread_n, NULL);
	}

	return 0;
}
