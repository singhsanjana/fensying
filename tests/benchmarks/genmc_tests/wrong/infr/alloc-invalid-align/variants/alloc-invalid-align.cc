#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

int user_main(int argc, char **argv)
{
	void *foo = aligned_alloc(3, 9);

	return 0;
}
