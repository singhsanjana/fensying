#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"
#include <genmc.h>

/* Some utilities */
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define __stringify_1(x) #x
#define __stringify(x) __stringify_1(x)

#define strlen(s) (sizeof(s)/sizeof(s[0]))

/* Test case parameters */
#define FILENAME foo.txt
#define BACKUPNAME FILENAME~

#define FILE_DATA "data"
#define BUFFER_DATA "modified data"

static const char file[] = __stringify(FILENAME);
static const char backup[] = __stringify(BACKUPNAME);

static const char initial_data[] = FILE_DATA;
static const char buffer_data[] = BUFFER_DATA;

/* Nano code + faked context */
#define MAKE_BACKUP 1

#include <fake.h>
#include <nano.c>

/* Test case */

void __VERIFIER_recovery_routine(void)
{
	int init_len = strlen(initial_data);
	int buf_len = strlen(buffer_data);
	int max_size = MAX(init_len, buf_len);
	char buf[MAX(init_len, buf_len)];

	int fd = open(__stringify(FILENAME), O_RDONLY, 0640);
	int fb = open(__stringify(BACKUPNAME), O_RDONLY, 0640);
	MODEL_ASSERT(fd != -1);

	/* int nr = lseek(fd, 0, SEEK_END); */
	int nr = read(fd, buf, max_size);

	/* DO NOT ASSERT ANYTHING AS THIS VERSION IS BUGGY!
	 * Just explore the whole state space...
	 * (see tests/wrong/fs/nano-backup-proc) */

	int nrb = lseek(fb, 0, SEEK_END);
}

int user_main()
{
	/* Create an existing file */
	int fd = creat(__stringify(FILENAME), 0640);
	int nw = write(fd, initial_data, strlen(initial_data));
	close(fd);

	/* Assume the file has persisted on disk */
	__VERIFIER_pbarrier();

	/* Fake nano's buffer creation and population */
	fake_make_new_buffer();

	/* Execute nano's writeout */
	bool ret = write_file(file, NULL, FALSE, OVERWRITE, FALSE);
	MODEL_ASSERT(ret);

	return 0;
}
