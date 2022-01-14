#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdatomic.h>

/* Make sure that CASes on unallocated memory doesn't confuse the interpreter */

int user_main()
{
	int r = 0;
	int a = atomic_compare_exchange_strong(__FILE__, __LINE__, (atomic_int *) 0xdeadbeef, &r, 42);
	return 0;
}
