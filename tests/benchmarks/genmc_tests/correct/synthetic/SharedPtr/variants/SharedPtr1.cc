#include <stdlib.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"
#include <stdatomic.h>

#include "../SharedPtr.c"

int main()
{
	thrd_t t1, t2;

	if (pthread_create(&t2, NULL, thread_one, NULL))
		abort();
	if (pthread_create(&t1, NULL, thread_two, NULL))
		abort();

	return 0;
}
