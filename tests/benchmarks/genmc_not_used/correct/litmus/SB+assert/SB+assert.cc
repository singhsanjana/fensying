#include "librace.h" 
#include "model-assert.h"
atomic_int x;
atomic_int y;

int r_x;
int r_y;

void *thread_1(void *arg)
{
	x = 1;
	r_y = y;
	;
}

void *thread_2(void *arg)
{
	y = 1;
	r_x = x;
	;
}
