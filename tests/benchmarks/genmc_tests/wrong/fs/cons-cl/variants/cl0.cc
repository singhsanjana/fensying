#include "librace.h" 
#include "model-assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdatomic.h>
#include <threads.h>

#include <fcntl.h>
#include <sys/stat.h>

void __VERIFIER_recovery_routine(void)
{
	/* printf("Nothing to do\n"); */
	return;
}

int user_main(int argc, char **argv)
{
	int ret = close(42);
	MODEL_ASSERT(ret != 1);

	return 0;
}
