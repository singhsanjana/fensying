#include "librace.h" 
#include "model-assert.h"
void *thread_1(void *unused)
{
	char buf[8];
	buf[0] = 42;

	int fd_x = open("x", O_WRONLY|O_SYNC, S_IRWXU);
	int fd_y = open("y", O_WRONLY|O_SYNC, S_IRWXU);

	write(fd_x, buf, 1);
	write(fd_y, buf, 1);
	;
}

void __VERIFIER_recovery_routine(void)
{
	char buf_x[8], buf_y[8];
	buf_x[1] = buf_y[0] = 1;

	int fd_y = open("y", O_RDONLY, S_IRWXU);
	int fd_x = open("x", O_RDONLY, S_IRWXU);

	if (fd_x == -1 || fd_y == -1)
		return;

	read(fd_y, buf_y, 1);
	read(fd_x, buf_x, 1);

	MODEL_ASSERT(!(buf_y[0] == 42 && buf_x[0] == 1));
	return;
}
