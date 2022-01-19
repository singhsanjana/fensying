#include "librace.h" 
#include "model-assert.h"
#include <stdatomic.h>
#include <threads.h>

#include "../rel-pporf.cc"

int user_user_user_main()
{
	thrd_t t[2];

	thrd_create(&t[0], (thrd_start_t)& , NULL);
	thrd_create(&t[1], (thrd_start_t)& , NULL);

	return 0;
}
