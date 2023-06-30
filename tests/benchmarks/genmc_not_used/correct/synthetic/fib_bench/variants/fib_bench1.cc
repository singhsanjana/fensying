#include "librace.h" 
#include "model-assert.h"
#include <threads.h>
#include <stdatomic.h>

#include "../fib_bench.cc"

int user_main()
{
	thrd_t t1, t2, t3;

	thrd_create(&t1, (thrd_start_t)& thread_1, NULL);
	thrd_create(&t3, (thrd_start_t)& thread_3, NULL);
	thrd_create(&t2, (thrd_start_t)& thread_2, NULL);

	return 0;
}
