#include <inttypes.h>
#include <threads.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <librace.h>

#include "mpmc-queue.h"

void threadA(struct mpmc_boundq_1_alt<int32_t, sizeof(int32_t)> *queue)
{
	int *bin = queue->write_prepare();
	if (bin == NULL) return;
	*bin = 1;
	queue->write_publish();
}

void threadB(struct mpmc_boundq_1_alt<int32_t, sizeof(int32_t)> *queue)
{
	int *bin;
	if ((bin = queue->read_fetch()) != NULL) {
		int k = *bin;
		queue->read_consume();
	}
}

void threadC(struct mpmc_boundq_1_alt<int32_t, sizeof(int32_t)> *queue)
{
	int *bin = queue->write_prepare();
	*bin = 1;
	queue->write_publish();

	if ((bin = queue->read_fetch()) != NULL) {
		int k = *bin;
		queue->read_consume();
	}
}

#define DEFAULT_READERS 2
#define DEFAULT_WRITERS 2
#define DEFAULT_RDWR 0

int readers = DEFAULT_READERS, writers = DEFAULT_WRITERS, rdwr = DEFAULT_RDWR;

int user_main(int argc, char **argv)
{
	struct mpmc_boundq_1_alt<int32_t, sizeof(int32_t)> queue;
	thrd_t A[DEFAULT_WRITERS], B[DEFAULT_READERS], C[DEFAULT_RDWR];

	/* Note: optarg() / optind is broken in model-checker - workaround is
	 * to just copy&paste this test a few times */
	//process_params(argc, argv);

	for (int i = 0; i < writers; i++)
		thrd_create(&A[i], (thrd_start_t)&threadA, &queue);
	for (int i = 0; i < readers; i++)
		thrd_create(&B[i], (thrd_start_t)&threadB, &queue);

	for (int i = 0; i < rdwr; i++)
		thrd_create(&C[i], (thrd_start_t)&threadC, &queue);

	for (int i = 0; i < writers; i++)
		thrd_join(A[i]);
	for (int i = 0; i < readers; i++)
		thrd_join(B[i]);
	for (int i = 0; i < rdwr; i++)
		thrd_join(C[i]);

	return 0;
}
