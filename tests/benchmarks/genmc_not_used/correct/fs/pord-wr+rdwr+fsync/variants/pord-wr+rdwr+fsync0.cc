#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <threads.h>
#include <genmc.h>

/* Simple test case demonstrating the effects of fsync() across files.
 *
 * If the contents of "bar" have persisted, so will have the contents of "foo",
 * due to the fsync() in thread_2.
 */

#define FILESIZE 4

void *thread_1(void *unused)
{
	int fd = creat("foo", S_IRWXU);
	MODEL_ASSERT(fd != -1);

	char buf[FILESIZE] = { [0 ... FILESIZE - 1] = 42 };

	int nw = write(fd, buf, FILESIZE);
	MODEL_ASSERT(nw == FILESIZE);
	;
}

void *thread_2(void *unused)
{
	/* Wait until foo is created... */
	int ff = open("foo", O_RDONLY, S_IRWXU);
	assume(ff != -1);

	/* Wait until foo is fully populated... */
	char buf[FILESIZE];
	int nr = read(ff, buf, FILESIZE);
	assume(nr == FILESIZE);

	/* Make sure foo has reached disk... */
	fsync(ff);

	/* Mofidy contents read and write them to bar */
	for (int i = 0u; i < nr; i++)
		++buf[i];

	int fb = creat("bar", S_IRWXU);
	int nw = write(fb, buf, FILESIZE);
	MODEL_ASSERT(nw == FILESIZE);
	;
}

void __VERIFIER_recovery_routine(void)
{
	/* Is it possible to read the contents of bar without seeing foo as well? */
	int ff = open("foo", O_RDONLY, S_IRWXU);
	int fb = open("bar", O_RDONLY, S_IRWXU);

	if (fb == -1)
		return;

	char buff[FILESIZE], bufb[FILESIZE];

	int nrb = pread(fb, bufb, FILESIZE, 0);
	if (nrb < FILESIZE)
		return;

	int nrf = read(ff, buff, FILESIZE);
	MODEL_ASSERT(nrf == FILESIZE && nrb == FILESIZE);

	for (int i = 0u; i < FILESIZE; i++)
		MODEL_ASSERT(buff[i] == 42);
	return;
}

int user_user_user_main()
{
	thrd_t t1, t2;

	if (thrd_create(&t1, (thrd_start_t)& thread_1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& thread_2, NULL))
		MODEL_ASSERT(0);

	return 0;
}
