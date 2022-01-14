#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <threads.h>
#include <genmc.h>

#include <fcntl.h>
#include <sys/stat.h>

#include "../op+op.cc"

int user_user_user_main()
{
	thrd_t t1, t2;

	/* Create the file */
	int fd = creat("foo", S_IRWXU);
	MODEL_ASSERT(fd != -1);

	__VERIFIER_pbarrier();

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
