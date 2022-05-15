#include <stdio.h>
#include <threads.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP        70
#define NUMREADERS  2

#include "barrier.h"



spinning_barrier *barr;
// int var = 0;
std::atomic<int> var;

void threadA(void *arg)
{
//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	// store_32(&var, 1);
	var.store(__FILE__, __LINE__, 1, std::memory_order_relaxed);
	barr->wait();
}

void threadB(void *arg)
{
//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	if (barr->wait()) {
		int l = var.load(__FILE__, __LINE__, std::memory_order_relaxed);
		// printf("var = %d\n", load_32(&var));
		MODEL_ASSERT(l != 0);
	}
}

int user_main(int argc, char **argv)
{
	thrd_t A, B[NUMREADERS];
	int i;
	std::atomic_init(&var, 0);

	barr = new spinning_barrier(NUMREADERS + 1);

	thrd_create(&A, (thrd_start_t)&threadA, NULL);
	for (i = 0; i < NUMREADERS; i++)
		thrd_create(&B[i], (thrd_start_t)&threadB, NULL);

	for (i = 0; i < NUMREADERS; i++)
		thrd_join(B[i]);
	thrd_join(A);

	return 0;
}
