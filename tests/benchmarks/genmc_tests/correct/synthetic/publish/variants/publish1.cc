#include <stdlib.h>
#include "librace.h" 
#include "model-assert.h"
#include <threads.h>#include <stdatomic.h>

#include "../publish.c"

int main()
{
	thrd_t tr, tw;

	if (pthread_create(&tr, NULL, thread_writer, NULL))
		abort();
	if (pthread_create(&tw, NULL, thread_reader, NULL))
		abort();

	return 0;
}
