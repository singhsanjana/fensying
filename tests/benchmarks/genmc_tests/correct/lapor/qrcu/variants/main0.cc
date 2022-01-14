#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

#include "../main.cc"

int user_user_user_main()
{
	thrd_t t1, t2, t3;

	init_qrcu_struct(&qs);

	if (thrd_create(&t1, (thrd_start_t)& qrcu_reader, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& qrcu_reader, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t3, (thrd_start_t)& qrcu_updater, NULL))
		MODEL_ASSERT(0);

	return 0;
}
