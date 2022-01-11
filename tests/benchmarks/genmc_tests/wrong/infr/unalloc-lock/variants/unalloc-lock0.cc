#include <stdlib.h>
#include <threads.h>#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"

/* Make sure that CASes on unallocated memory doesn't confuse the interpreter */

int main()
{
	pthread_mutex_lock((pthread_mutex_t *) 0xdeadbeef);
	return 0;
}
