#include "librace.h" 
#include "model-assert.h"
typedef int elem_t;

elem_t myread(elem_t *loc);
void mywrite(elem_t *loc, elem_t val);
void myinit(elem_t *loc, elem_t val);

elem_t x;

void *thread_1(void *unused)
{
	mywrite(&x, 1);
	;
}

void *thread_2(void *unused)
{
	myread(&x);
	;
}

void *thread_3(void *unused)
{
	myread(&x);
	;
}
