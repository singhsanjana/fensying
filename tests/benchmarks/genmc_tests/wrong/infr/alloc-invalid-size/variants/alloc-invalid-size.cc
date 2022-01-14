#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>

int user_main()
{
	void *foo = malloc(-2);

	return 0;
}
