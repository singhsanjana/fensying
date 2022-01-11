#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <threads.h>#include <stdatomic.h>
#include <stdbool.h>
#include "librace.h" 
#include "model-assert.h"
#include <genmc.h>

/* user_main() reads a file in batches, dispatches the batches to different
 * threads for processing which in turn write the result of some
 * computation in different files. In the end, user_main() glues all
 * different files in one.
 *
 * After a crash, only a prefix of the file written might have persisted,
 * but the prefix should be in accordance with the order the file
 * was written.
 */

#define NUM_WRITERS 4
#define BATCH_SIZE 4
#define FILESIZE BATCH_SIZE * NUM_WRITERS

struct thread_info_struct {
	thrd_t tid; /* POSIX thread id, as returned by the library */
	int thrid; /* Application-defined thread id */
	int thrcnt;
	_Atomic(char *) data;
	atomic_bool ready;
};

struct thread_info_struct threads[NUM_WRITERS + 1];

/* We need ugly trickery to have many threads writing to different
 * files in a parametric fashion with const static names... */
#define DECLARE_WORKER(id)						\
	void *thread_##id(void *arg)					\
	{								\
	struct thread_info_struct *thr = arg;				\
									\
	int fr = open("thefile", O_RDONLY, 0640);			\
	MODEL_ASSERT(fr != -1);						\
	int fw = creat("file" #id, 0640);				\
	MODEL_ASSERT(fw != -1);						\
									\
	/* Wait until data to process is ready */			\
	__VERIFIER_assume(atomic_load_explicit(__LINE__, &thr->ready, memory_order_acquire)); \
	char *buf = atomic_load_explicit(__LINE__, &thr->data, memory_order_relaxed); \
									\
	/* Edit the read data (model a computation) */			\
	for (int i = 0u; i < BATCH_SIZE; i++)				\
		buf[i] = buf[i] + thr->thrid;				\
	int nw = write(fw, buf, BATCH_SIZE);				\
	MODEL_ASSERT(nw == BATCH_SIZE);					\
	return NULL;							\
	}

DECLARE_WORKER(1);
DECLARE_WORKER(2);
DECLARE_WORKER(3);
DECLARE_WORKER(4);

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

#define SPAWN_WORKER(id)						\
	do {								\
		threads[id].thrid = id;					\
		threads[id].data = NULL;				\
		threads[id].ready = false;				\
		threads[id].thrcnt = NUM_WRITERS;			\
		if (thrd_create(&threads[id].tid, (thrd_start_t)& thread_##id, NULL)) \
			abort();					\
	} while (0)

#define READ_NOTIFY(id)						\
	char buf##id[BATCH_SIZE];				\
	int nr##id = read(fd, buf##id, BATCH_SIZE);		\
	MODEL_ASSERT(nr##id == BATCH_SIZE);				\
	atomic_store_explicit(__LINE__, &threads[id].data, buf##id, memory_order_relaxed); \
	atomic_store_explicit(__LINE__, &threads[id].ready, true, memory_order_release);

#define JOIN_WORKER(id)					\
	if (thrd_join(threads[id].tid))	\
		abort();

#define GLUE_RESULTS(id, fc)				\
	char res##id[BATCH_SIZE];			\
	int f##id = open("file" #id, O_RDONLY, 0640);	\
	int n##id = read(f##id, res##id, BATCH_SIZE);	\
	int nw##id = write(fc, res##id, BATCH_SIZE);

int user_main()
{
	thrd_t t[NUM_WRITERS + 1];

	/* Create and populate a "big" file (supposedly exists on disk) */
	char buf[FILESIZE] = { [0 ... FILESIZE - 1] = 42 };

	int fd = open("thefile", O_CREAT|O_RDWR, 0640);
	int nw = write(fd, buf, FILESIZE);
	MODEL_ASSERT(nw == FILESIZE);
	lseek(fd, 0, SEEK_SET); /* reset offset */
	int fc = creat("result", 0640);
	__VERIFIER_pbarrier();

	/* Spawn worker threads */
	SPAWN_WORKER(1);
	SPAWN_WORKER(2);
	SPAWN_WORKER(3);
	SPAWN_WORKER(4);

	/* Read file and notify workers */
	READ_NOTIFY(1);
	READ_NOTIFY(2);
	READ_NOTIFY(3);
	READ_NOTIFY(4);

	/* Wait for workers to finish and glue the results */
	JOIN_WORKER(1);
	JOIN_WORKER(2);
	JOIN_WORKER(3);
	JOIN_WORKER(4);

	GLUE_RESULTS(1, fc);
	GLUE_RESULTS(2, fc);
	GLUE_RESULTS(3, fc);
	GLUE_RESULTS(4, fc);

	return 0;
}
