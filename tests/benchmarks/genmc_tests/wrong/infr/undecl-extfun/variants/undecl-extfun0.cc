#include "librace.h" 
#include "model-assert.h"
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"

/* Fail gracefully when an undeclared external function is called */

int user_main()
{
	my_function(42);
	return 0;
}
