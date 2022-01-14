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

void __VERIFIER_recovery_routine(void)
{
	char buf[8];

	int fd = open("foo", O_RDONLY, 0666);
	MODEL_ASSERT(fd != -1);

	int nr = pread(fd, buf, 4, 0);
	if (nr < 4)
		return;

	/* Is is possible to read the second block w/out the first? */
	MODEL_ASSERT(!(buf[2] == 2 && buf[3] == 2 &&
				     buf[0] == 1 && buf[1] == 1));
	return;
}

int user_user_user_main()
{
	char buf[8];

	buf[0] = 1;
	buf[1] = 1;
	buf[2] = 2;
	buf[3] = 2;
	buf[4] = 2;
	buf[5] = 2;

	int fd = open("foo", O_CREAT|O_TRUNC|O_RDWR, S_IRWXU);
	pwrite(fd, buf, 2, 0);

	__VERIFIER_pbarrier();

	pwrite(fd, buf + 2, 4, 0); /* overwrite + append */

	close(fd);

	return 0;
}
