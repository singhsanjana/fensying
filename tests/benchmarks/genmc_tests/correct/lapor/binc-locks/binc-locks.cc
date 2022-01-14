#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex lx;
std::mutex ly;

int x;
int y;

void *thread_n(void *unused)
{
	lx.lock();
	x = x + 1;
	lx.unlock();

	ly.lock();
	y = y + 1;
	ly.unlock();
	;
}
