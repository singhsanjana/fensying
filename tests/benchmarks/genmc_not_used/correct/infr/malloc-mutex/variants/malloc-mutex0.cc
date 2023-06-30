#include "librace.h" 
#include "model-assert.h"
#include <mutex>
#include <stdio.h>
#include <threads.h>
#include <stdlib.h>

typedef struct {
	std::mutex m;
} lock;

int user_user_user_main()
{
	lock *t = malloc(sizeof(lock));

	pthread_mutex_init(&t->m, NULL);
	t->m.lock();

	return 0;
}
