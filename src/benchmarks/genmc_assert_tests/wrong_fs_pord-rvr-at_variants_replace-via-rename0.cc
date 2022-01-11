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

	int fd = open("bar", O_RDONLY, 0666);

	if (fd == -1)
		return;

	int nr = read(fd, buf, 1);

	/* Is is possible to see bar as an empty file? */
	MODEL_ASSERT(nr != 0);
	return;
}

int user_main()
{
	int fd = open("foo", O_CREAT|O_TRUNC|O_RDWR, S_IRWXU);

	char buf[8];
	buf[0] = 42;
	write(fd, buf, 1);

	close(fd);
	rename("foo", "bar");

	return 0;
}
