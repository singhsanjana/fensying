#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>

int user_main()
{
	void *foo = aligned_alloc(3, 9);

	return 0;
}
