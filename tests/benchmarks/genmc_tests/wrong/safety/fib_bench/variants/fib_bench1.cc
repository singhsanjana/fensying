#include "librace.h" 
#include "model-assert.h"
#include <threads.h>
#include <stdatomic.h>

#include "../fib_bench.cc"

int user_main(int argc, char **argv)
{
	thrd_t id1, id2, id3;
	atomic_init(&i, 1);
	atomic_init(&j, 1);

	thrd_create(&id2, (thrd_start_t)& t2, NULL);
	thrd_create(&id1, (thrd_start_t)& t1, NULL);
	thrd_create(&id3, (thrd_start_t)& t3, NULL);

	return 0;
}
