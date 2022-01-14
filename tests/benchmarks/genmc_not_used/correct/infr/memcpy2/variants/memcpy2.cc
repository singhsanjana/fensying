#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

int user_user_main()
{
	char a[5] = "xxxx\0";

	return !(a[0] == 'x' && a[1] == 'x');
}
