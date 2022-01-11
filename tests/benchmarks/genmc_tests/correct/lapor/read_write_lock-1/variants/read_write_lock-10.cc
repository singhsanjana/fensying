#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"

#include "../read_write_lock-1.c"

int main()
{
	thrd_t t1, t2, t3, t4;

	if (pthread_create(&t1, NULL, writer, NULL))
		abort();
	if (pthread_create(&t2, NULL, reader, NULL))
		abort();
	if (pthread_create(&t3, NULL, writer, NULL))
		abort();
	if (pthread_create(&t4, NULL, reader, NULL))
		abort();

	/* thrd_join(t1); */
	/* thrd_join(t2); */
	/* thrd_join(t3); */
	/* thrd_join(t4); */

	return 0;
}
