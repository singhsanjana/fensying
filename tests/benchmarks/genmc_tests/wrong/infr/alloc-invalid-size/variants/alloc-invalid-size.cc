#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>

int user_main(int argc, char **argv)
{
	void *foo = malloc(-2);

	return 0;
}
