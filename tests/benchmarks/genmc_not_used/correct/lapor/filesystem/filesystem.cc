#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#define NUMBLOCKS 26
#define NUMINODES 32

std::mutex locki[NUMINODES];
int inode[NUMINODES];

std::mutex lockb[NUMBLOCKS];
bool busy[NUMBLOCKS];

int idx[N];

void *thread_n(void *arg)
{
	int tid = *((int *) arg);
	int i = tid % NUMINODES;

	locki[i].lock();
	if (inode[i] == 0) {
		int b = (i * 2) % NUMBLOCKS;
		while (true) {
			lockb[b].lock();
			if (!busy[b]) {
				busy[b] = true;
				inode[i] = b + 1;
				lockb[b].unlock();
				break;
			}
			lockb[b].unlock();
			b = (b + 1) % NUMBLOCKS;
		}
	}
	locki[i].unlock();
	;
}
