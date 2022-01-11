#include <stdlib.h>
#include <stdatomic.h>
#include "librace.h" 
#include "model-assert.h"

/* Make sure that CASes on unallocated memory doesn't confuse the interpreter */

int main()
{
	int r = 0;
	int a = atomic_compare_exchange_strong((atomic_int *) 0xdeadbeef, &r, 42);
	return 0;
}
