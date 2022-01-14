#include "librace.h" 
#include "model-assert.h"
#include "barrier.h"

#ifndef NUMREADERS
# define NUMREADERS 1
#endif

DEFINE_BARRIER(barr, NUMREADERS + 1);

int var = 0;

void *threadW(void *unused)
{
	var = 42;
	wait(&barr);
	;
}

void *threadR(void *unused)
{
	wait(&barr);
	int data = var;
	;
}
