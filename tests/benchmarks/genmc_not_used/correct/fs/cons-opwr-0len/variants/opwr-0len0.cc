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
	/* printf("Nothing to do\n"); */
	return;
}

int user_user_user_main()
{
	char buf[3] = "000";

	int fd = open("foo", O_CREAT|O_WRONLY, S_IRWXU);
	MODEL_ASSERT(fd != -1);

	int nw = write(fd, buf, 3);
	MODEL_ASSERT(nw == 3);

	write(fd, buf, -1);

	return 0;
}
