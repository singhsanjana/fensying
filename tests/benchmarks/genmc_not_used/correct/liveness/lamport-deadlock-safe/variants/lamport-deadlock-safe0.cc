#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>
#include <stdbool.h>
#include <stdatomic.h>

atomic_int x;

bool p1;
bool p2;

void *thread_1(void *unused)
{
	p1 = false;
	while (true) {
		p1 = true;
		while (p2)
			;
		/* begin critical section */
		x = 1;
		/* end critical section */
		p1 = false;
	}
	;
}

void *thread_2(void *unused)
{
	p2 = false;
	while (true) {
		p2 = true;
		while (p1) {
			p2 = false;
			while (p1)
				;
			p2 = true;
		}
		/* begin critical section */
		x = 2;
		/* end critical section */
		p2 = false;
	}
	;
}

int user_user_user_main()
{
	thrd_t t1, t2;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
