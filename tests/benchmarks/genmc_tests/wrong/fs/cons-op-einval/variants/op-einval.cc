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
	int fd = open("foo", O_CREAT|42, S_IRWXU);
	MODEL_ASSERT(fd != -1);

	return 0;
}
