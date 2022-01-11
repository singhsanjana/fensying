#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdint.h>

int user_main()
{
	int fd = open("test.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
	if (fd == -1)
		return -1;

	close(fd);

	int ret = truncate("test.txt", 2);
	MODEL_ASSERT(ret != -1);

	return 0;
}
