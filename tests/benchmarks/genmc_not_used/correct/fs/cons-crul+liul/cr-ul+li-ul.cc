#include "librace.h" 
#include "model-assert.h"
void __VERIFIER_recovery_routine(void)
{
	/* printf("Nothing to do\n"); */
	return;
}

void *thread_1(void *unused)
{
	int fd = creat("foo", S_IRWXU);
	MODEL_ASSERT(fd != -1);
	unlink("foo");
	;
}

void *thread_2(void *unused)
{
	link("foo", "bar");
	unlink("bar");
	;
}
