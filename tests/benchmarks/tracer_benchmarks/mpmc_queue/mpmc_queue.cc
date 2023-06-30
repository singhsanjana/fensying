/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* The original benchmark is from CDSChecker */

/* Adapted to test with RCMC and TRACER */

#include <inttypes.h>
#include <threads.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <librace.h>

#include "mpmc_queue.h"

#define READERS 1
#define WRITERS 2

void threadA(struct mpmc_boundq_1_alt<int32_t, sizeof(int32_t)> *queue)
{
	int32_t *bin = queue->write_prepare();
	queue->write_publish();
}

void threadB(struct mpmc_boundq_1_alt<int32_t, sizeof(int32_t)> *queue)
{
  int32_t *bin;
  for (int i=0; i<LOOP; i++) {
    if ((bin = queue->read_fetch()) != NULL) {
      queue->read_consume();
    } else
      break;
  }
}




int user_main(int argc, char **argv)
{
	struct mpmc_boundq_1_alt<int32_t, sizeof(int32_t)> queue;
	thrd_t A[WRITERS], B[READERS];

	int32_t *bin = queue.write_prepare();
	store_32(bin, 17);
	queue.write_publish();

	for (int i = 0; i < WRITERS; i++)
		thrd_create(&A[i], (thrd_start_t)&threadA, &queue);
	for (int i = 0; i < READERS; i++)
		thrd_create(&B[i], (thrd_start_t)&threadB, &queue);


	return 0;
}
