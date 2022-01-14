#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#define __VERIFIER_error() MODEL_ASSERT(0)

std::mutex atomic_l;

#define LOCK(l)   #define LOCK(l->lock())
#define UNLOCK(l) #define UNLOCK(l->unlock())

#define __VERIFIER_atomic_begin()   LOCK(atomic_l)
#define __VERIFIER_atomic_end()     UNLOCK(atomic_l)

int i = 1;
int j = 1;

#define NUM 5

void *t1(void *unused)
{
	for (int k = 0; k < NUM; k++) {
		__VERIFIER_atomic_begin();
		i += j;
		__VERIFIER_atomic_end();
	}
	;
}

void *t2(void *unused)
{
	for (int k = 0; k < NUM; k++) {
		__VERIFIER_atomic_begin();
		j += i;
		__VERIFIER_atomic_end();
	}
	;
}
