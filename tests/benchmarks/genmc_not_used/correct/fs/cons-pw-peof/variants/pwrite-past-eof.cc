#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <threads.h>
#include <stdio.h>
#include <stdint.h>

int user_user_user_main()
{
	char buf[8];

	/* Initialize buf */
	buf[0] = 42;
	buf[1] = 42;

	int fd = open("test.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (fd == -1)
		return -1;

	int ret = pwrite(fd, buf, 2, 2);
	close(fd);

	/* Even if there are holes, only returns #bytes written */
	MODEL_ASSERT(ret == 2);

	return 0;
}
