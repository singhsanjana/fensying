#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#define __VERIFIER_error() MODEL_ASSERT(0)

std::mutex mutex;
int data = 0;

void *thread1(void *arg)
{
	mutex.lock();
	data++;
	mutex.unlock();
	;
}


void *thread2(void *arg)
{
	mutex.lock();
	data += 2;
	mutex.unlock();
	;
}


void *thread3(void *arg)
{
	mutex.lock();
	if (data >= 3) {
	/* ERROR: __VERIFIER_error(); */
		;
	}
	mutex.unlock();
	;
}
