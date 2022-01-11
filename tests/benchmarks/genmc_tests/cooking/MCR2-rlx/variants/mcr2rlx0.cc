#include <stdio.h>
#include <stdlib.h>
#include "librace.h" 
#include "model-assert.h"
#include <threads.h>#include "../../../stdatomic.h"

#include "../mcr2rlx.c"

int main(){
	thrd_t t1, t2, t3;

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();
	if (pthread_create(&t2, NULL, thread_2, NULL))
		abort();
	if (pthread_create(&t3, NULL, thread_3, NULL))
		abort();

	if (pthread_join(t1, NULL))
		abort();
	if (pthread_join(t2, NULL))
		abort();
	if (pthread_join(t3, NULL))
		abort();

	return 0;
}
