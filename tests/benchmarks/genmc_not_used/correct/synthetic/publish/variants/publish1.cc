#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../publish.cc"

int user_main()
{
	thrd_t tr, tw;

	if (thrd_create(&tr, (thrd_start_t)& thread_writer, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&tw, (thrd_start_t)& thread_reader, NULL))
		MODEL_ASSERT(0);

	return 0;
}
