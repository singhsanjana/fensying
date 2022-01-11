#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"
#include <genmc.h>

#include <fcntl.h>
#include <sys/stat.h>

void __VERIFIER_recovery_routine(void)
{
	char buf[2];

	int fd = open("foo", O_RDONLY, 0666);

	pread(fd, &buf[0], 2, 0);

	/* Are overwrites atomic? */
	MODEL_ASSERT(!(buf[0] == '1' && buf[1] == '0'));
	return;
}

int user_main()
{
	char buf[4] = "0011";

	/* Initialize foo */
	int fd = creat("foo", 0640);
	write(fd, buf, 2);

	__VERIFIER_pbarrier();

	/* Write to different offsets */
	pwrite(fd, buf + 2, 2, 0);

	close(fd);

	return 0;
}
