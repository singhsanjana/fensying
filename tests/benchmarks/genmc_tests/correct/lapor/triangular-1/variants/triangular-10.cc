#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <threads.h>

#include "../triangular-1.cc"

int user_user_user_main()
{
	thrd_t id1, id2;

	if (thrd_create(&id1, (thrd_start_t)& t1, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&id2, (thrd_start_t)& t2, NULL))
		MODEL_ASSERT(0);

	__VERIFIER_atomic_begin();
	int condI = i > LIMIT;
	__VERIFIER_atomic_end();

	__VERIFIER_atomic_begin();
	int condJ = j > LIMIT;
	__VERIFIER_atomic_end();

	if (condI || condJ)
		__VERIFIER_error();

	return 0;
}
