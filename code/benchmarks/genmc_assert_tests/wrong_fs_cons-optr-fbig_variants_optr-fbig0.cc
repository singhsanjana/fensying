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

void __VERIFIER_recovery_routine(void)
{
	/* printf("Nothing to do\n"); */
	return;
}

int user_main()
{
	char buf[8];
	buf[0] = 42;

	int fd = open("foo", O_CREAT|O_RDONLY, S_IRWXU);
	MODEL_ASSERT(fd != -1);
	close(fd);

	int ret = truncate("foo", 4242);
	MODEL_ASSERT(ret >= 0);

	return 0;
}
