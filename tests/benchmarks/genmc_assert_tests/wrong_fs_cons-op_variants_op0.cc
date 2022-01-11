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

atomic_int x;
atomic_int y;

void __VERIFIER_recovery_routine(void)
{
	/* printf("Nothing to do\n"); */
	return;
}

int user_main()
{
	int fd = open("foo", O_TRUNC, S_IRWXU);
	MODEL_ASSERT(fd != -1);

	return 0;
}
