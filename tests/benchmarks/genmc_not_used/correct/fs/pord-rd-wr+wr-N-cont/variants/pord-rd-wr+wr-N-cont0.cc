#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <threads.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <genmc.h>

/*
 * user_user_user_main() reads a file in batches and dispatches the batches to different
 * threads for processing. The threads write the result of some
 * computation in the same file.
 *
 * After a crash, only a prefix of the file written might have persisted.
 */

#define NUM_WRITERS 3
#define BATCH_SIZE 4
#define FILESIZE BATCH_SIZE * NUM_WRITERS

struct thread_info_struct {
	thrd_t tid; /* POSIX thread id, as returned by the library */
	int thrid; /* Application-defined thread id */
	int thrcnt;
	_Atomic(char *) data;
	bool ready;
};

struct thread_info_struct threads[NUM_WRITERS];

void *thread_n(void *arg)
{
	struct thread_info_struct *thr = arg;

	int fr = open("thefile", O_RDONLY, 0640);
	MODEL_ASSERT(fr != -1);
	int fw = creat("result", 0640);
	MODEL_ASSERT(fw != -1);

	/* Wait until data to process is ready */
	__VERIFIER_assume(atomic_load_explicit(__FILE__, __LINE__, &thr->ready, memory_order_acquire));
	char *buf = atomic_load_explicit(__FILE__, __LINE__, &thr->data, memory_order_relaxed);

	/* Edit the read data (model a computation) */
	for (int i = 0u; i < BATCH_SIZE; i++)
		buf[i] = buf[i] + thr->thrid;

	/* Write the result directly */
	int nw = write(fw, buf, BATCH_SIZE);
	MODEL_ASSERT(nw == BATCH_SIZE);
	;
}

void __VERIFIER_recovery_routine(void)
{
	/* Observe the outcome of the serialization */
	int fd = open("result", O_RDONLY, S_IRWXU);
	MODEL_ASSERT(fd != -1);

	char buf[FILESIZE];
	int nr = read(fd, buf, FILESIZE);
	MODEL_ASSERT(nr <= FILESIZE);

	/* /\* Print the file contents *\/ */
	/* if (nr == FILESIZE) { */
	/* 	for (int i = 0u; i < FILESIZE; i++) */
	/* 		printf("%d ", buf[i]); */
	/* 	printf("\n"); */
	/* } */
	return;
}

int user_user_user_main()
{
	thrd_t t[NUM_WRITERS];

	/* Create and populate a "big" file (supposedly exists on disk) */
	char init[FILESIZE] = { [0 ... FILESIZE - 1] = 42 };

	int fd = open("thefile", O_CREAT|O_RDWR, 0640);
	int nw = write(fd, init, FILESIZE);
	MODEL_ASSERT(nw == FILESIZE);
	lseek(fd, 0, SEEK_SET); /* reset offset */
	int fc = creat("result", 0640);
	__VERIFIER_pbarrier();

	/* Spawn worker threads */
	for (int i = 0u; i < NUM_WRITERS; i++) {
		threads[i].thrid = i;
		threads[i].data = NULL;
		threads[i].ready = false;
		threads[i].thrcnt = NUM_WRITERS;
		if (thrd_create(&threads[i].tid, (thrd_start_t)& thread_n, NULL))
			MODEL_ASSERT(0);
	}

	/* Read file and notify workers */
	char buf[NUM_WRITERS][BATCH_SIZE];
	int count = 0u;
	for (int i = 0u; i < FILESIZE; i += BATCH_SIZE, ++count) {
		int nr = read(fd, buf[count], BATCH_SIZE);
		MODEL_ASSERT(nr == BATCH_SIZE);
		atomic_store_explicit(__FILE__, __LINE__, &threads[count].data, buf[count], memory_order_relaxed);
		atomic_store_explicit(__FILE__, __LINE__, &threads[count].ready, true, memory_order_release);
	}

	/* Wait for workers to finish and glue the results */
	for (int i = 0u; i < NUM_WRITERS; i++) {
		if (thrd_join(threads[i].tid))
			MODEL_ASSERT(0);
	}

	return 0;
}
