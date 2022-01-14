#include "librace.h" 
#include "model-assert.h"
#include <mutex>
std::mutex atomic_l;

#define __VERIFIER_atomic_begin() #define __VERIFIER_atomic_begin(->lock()
#define __VERIFIER_atomic_end()   #define __VERIFIER_atomic_end(->unlock()

#define __VERIFIER_error() MODEL_ASSERT(0)

int i = 3;
int j = 6;

#ifndef NUM
# define NUM 5
#endif
#define LIMIT (2 * NUM + 6)

void *t1(void *unused)
{
	for (int k = 0; k < NUM; k++) {
		__VERIFIER_atomic_begin();
		i = j + 1;
		__VERIFIER_atomic_end();
	}
	;
}

void *t2(void *unused)
{
	for (int k = 0; k < NUM; k++) {
		__VERIFIER_atomic_begin();
		j = i + 1;
		__VERIFIER_atomic_end();
	}
	;
}
