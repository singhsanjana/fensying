#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"
#include <genmc.h>

#include <fcntl.h>
#include <sys/stat.h>

#define WRITE_FILE(i)							\
do {								        \
	pwrite(fd, buf, 2, i);						\
} while (0)

void __VERIFIER_recovery_routine(void)
{
	char buf[4];

	int fd = open("foo", O_RDONLY, 0666);

	/* Read the first block */
	int nr = pread(fd, buf, 1, 0);
	if (nr > 0)
		MODEL_ASSERT(buf[0] == '0' || buf[0] == '1');
	return;
}

int user_main()
{
	char buf0[12] = "000000000000";
	char buf[4] = "1111";

	int fd = open("foo", O_CREAT|O_TRUNC|O_RDWR, S_IRWXU);

	write(fd, buf0, 10);

	__VERIFIER_pbarrier();

	WRITE_FILE(0);
	WRITE_FILE(2);
	WRITE_FILE(4);
	WRITE_FILE(6);
	WRITE_FILE(8);

	return 0;
}
