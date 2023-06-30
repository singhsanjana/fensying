#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../read_write_lock-1.cc"

int user_user_user_main()
{
	thrd_t t1, t2, t3, t4;

	if (thrd_create(&t1, (thrd_start_t)& writer, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& reader, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t3, (thrd_start_t)& writer, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t4, (thrd_start_t)& reader, NULL))
		MODEL_ASSERT(0);

	/* thrd_join(t1); */
	/* thrd_join(t2); */
	/* thrd_join(t3); */
	/* thrd_join(t4); */

	return 0;
}
