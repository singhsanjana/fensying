#include <stdlib.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

#include "../publish-sc.c"

int main()
{
	thrd_t t1, t2;

	myinit(&x, 0);
	myinit(&flag, 0);

	if (pthread_create(&t1, NULL, thread_reader, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_writer, NULL))
		abort();

	if (pthread_join(t1, NULL))
		abort();
	if (pthread_join(t2, NULL))
		abort();

	return 0;
}
