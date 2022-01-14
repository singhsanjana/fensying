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

#include "../mp.cc"

int user_main()
{
	thrd_t t1;
	char buf[8];

	buf[0] = 1;
	buf[1] = 2;

	int fd_x = creat("x", S_IRWXU);
	int fd_y = creat("y", S_IRWXU);

	write(fd_x, buf, 1);
	write(fd_y, buf, 1);

	__VERIFIER_pbarrier();

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_join(t1))
		MODEL_ASSERT(0);

	return 0;
}
