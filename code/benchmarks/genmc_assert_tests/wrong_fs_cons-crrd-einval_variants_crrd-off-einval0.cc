#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <threads.h>#include <genmc.h>
#include "librace.h" 
#include "model-assert.h"
#include "librace.h" 
#include "model-assert.h"

#include <fcntl.h>
#include <sys/stat.h>

int user_main()
{
	char buf[8];
	buf[0] = 42;

	int fd = open("foo", O_CREAT|O_RDONLY, S_IRWXU);
	MODEL_ASSERT(fd != -1);

	int nr = pread(fd, buf, 1, -42);
	MODEL_ASSERT(nr >= 0);

	return 0;
}
