#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <threads.h>

#include <fcntl.h>
#include <sys/stat.h>

int user_main(int argc, char **argv)
{
	/* Create the file */
	int fd = creat("foo", S_IRWXU);
	close(fd);

	/* Open the file with O_RDONLY|O_TRUNC */
	fd = open("foo", O_RDONLY|O_TRUNC, 0640);
	MODEL_ASSERT(fd != -1);
	close(fd);

	return 0;
}
