#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int x;
atomic_int y;

void *thread_1(void *unused)
{
	while (1) {
	  if (x) break;
	  if (y) break;
        } ;
	;
}

void *thread_2(void *unused)
{
	if (!x) y = 1;
	;
}

void *thread_3(void *unused)
{
	if (!y) x = 1;
	;
}

int user_user_user_main()
{
	thrd_t t1, t2, t3;
	x = 0;
	y = 0;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t3, (thrd_start_t)& thread_3, NULL))
		MODEL_ASSERT(0);

	return 0;
}
