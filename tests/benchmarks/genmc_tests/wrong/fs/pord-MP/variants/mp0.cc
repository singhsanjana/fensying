#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <threads.h>#include <genmc.h>
#include "librace.h" 
#include "model-assert.h"

#include <fcntl.h>
#include <sys/stat.h>

#include "../mp.c"

int main()
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

	if (pthread_create(&t1, NULL, thread_1, NULL))
		abort();
	if (pthread_join(t1, NULL))
		abort();

	return 0;
}
