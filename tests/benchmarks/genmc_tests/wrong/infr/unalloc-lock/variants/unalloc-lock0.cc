#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>

/* Make sure that CASes on unallocated memory doesn't confuse the interpreter */

int user_main()
{
				pthread_mutex_lock((std::mutex *->lock(->lock() 0xdeadbeef);
	return 0;
}
