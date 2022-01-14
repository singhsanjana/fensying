#include "librace.h" 
#include "model-assert.h"
typedef int elem_t;

void myinit(elem_t *loc, elem_t val);
void mywrite(elem_t *loc, elem_t val);
elem_t myread(elem_t *loc);

elem_t x;

void *thread_1(void *unused)
{
	myread(&x);
	mywrite(&x, 1);
	;
}

void *thread_2(void *arg)
{
	mywrite(&x, 2);
	;
}
