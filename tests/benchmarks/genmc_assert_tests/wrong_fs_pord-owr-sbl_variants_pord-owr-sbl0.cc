#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <threads.h>#include <genmc.h>
#include "librace.h" 
#include "model-assert.h"

#include <fcntl.h>
#include <sys/stat.h>

void __VERIFIER_recovery_routine(void)
{
	char buf[8];
	buf[0] = 0;
	buf[1] = 0;

	int fd = open("foo", O_RDONLY, 0666);

	if (fd == -1)
		return;

	int nr = pread(fd, buf, 2, 0);
	if (nr < 2)
		return;

	/* Is is possible to read {2,1} ? */
	MODEL_ASSERT(!(buf[0] == 2 && buf[1] == 1));
	return;
}

int user_main()
{
	char buf1[8], buf2[8];

	buf1[0] = 1;
	buf1[1] = 1;
	buf2[0] = 2;
	buf2[1] = 2;

	int fd = open("foo", O_CREAT|O_TRUNC|O_RDWR, S_IRWXU);

	__VERIFIER_pbarrier();

	pwrite(fd, buf1, 2, 0);
	pwrite(fd, buf2, 2, 0);

	close(fd);

	return 0;
}
