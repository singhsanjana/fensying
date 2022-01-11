#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"

#include "../names-2d-array.c"

int main()
{
	thrd_t t1, t2;

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_2, NULL))
		abort();

	if (pthread_join(t1, NULL))
		abort();
	if (pthread_join(t2, NULL))
		abort();

	if (a[1][1] == 42 && stack.nodes[1][1].value == 42)
		assert(0);

	return 0;
}
