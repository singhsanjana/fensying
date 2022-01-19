#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#define __VERIFIER_error() MODEL_ASSERT(0)

#define SIZE  128
#define MAX   4
#define N  13

int table[SIZE];
std::mutex  cas_mutex[SIZE];

thrd_t  tids[N];
int idx[N];

bool cas(int *tab, int h, int val, int new_val)
{
	int ret_val = false;

	cas_mutex[h].lock();

	if (tab[h] == val) {
		tab[h] = new_val;
		ret_val = true;
	}

	cas_mutex[h].unlock();

	return ret_val;
}



void *thread_routine(void *arg)
{
	int m = 0, w, h;
	int tid = *((int *) arg);

	while (true) {
		if (m < MAX) {
			w = (++m) * 11 + tid;
		} else {
			;
		}

		h = (w * 7) % SIZE;

		if (h < 0) {
		ERROR: __VERIFIER_error();
		}

		while (cas(table, h, 0, w) == false) {
			h = (h + 1) % SIZE;
		}
	}

}
