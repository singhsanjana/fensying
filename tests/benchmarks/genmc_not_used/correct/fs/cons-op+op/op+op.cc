#include "librace.h" 
#include "model-assert.h"
void __VERIFIER_recovery_routine(void)
{
	/* printf("Nothing to do\n"); */
	return;
}

void *thread_1(void *unused)
{
	int fd = open("foo", O_RDONLY, S_IRWXU);
	MODEL_ASSERT(fd != -1);
	;
}

void *thread_2(void *unused)
{
	int fd = open("foo", O_RDONLY, S_IRWXU);
	MODEL_ASSERT(fd != -1);
	;
}
