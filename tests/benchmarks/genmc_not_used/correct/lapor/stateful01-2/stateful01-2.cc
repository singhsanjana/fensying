#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#define __VERIFIER_error() MODEL_ASSERT(0)

std::mutex ma, mb;
int data1, data2;

void *thread1(void *arg)
{
	ma.lock();
	data1++;
	ma.unlock();

	ma.lock();
	data2++;
	ma.unlock();

	;
}


void *thread2(void *arg)
{
	ma.lock();
	data1 += 5;
	ma.unlock();

	ma.lock();
	data2 -= 6;
	ma.unlock();

	;
}
