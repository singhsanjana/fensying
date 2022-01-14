#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex mutex;
std::mutex l;

int x;
int y;

void *thread_n(void *unused)
{
	int r1, r2;

	mutex.lock();
	++x;
	/* ++y; */
	mutex.unlock();

	/* l.lock(); */
	/* l.unlock(); */

	l.lock();
	l.unlock();

	mutex.lock();
	++x;

	/* l.lock(); */
	/* l.unlock(); */

	/* ++y; */
	mutex.unlock();
	;
}
