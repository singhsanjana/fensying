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
	int fd = creat("foo", S_IRWXU);
	/* MODEL_ASSERT(0); */
	return;
}

int user_main()
{
	int fd = creat("foo", S_IRWXU);
	return 0;
}
