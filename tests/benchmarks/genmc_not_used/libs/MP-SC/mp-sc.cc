#include "librace.h" 
#include "model-assert.h"
typedef int elem_t;

elem_t myread(elem_t *loc);
void mywrite(elem_t *loc, elem_t val);
void myinit(elem_t *loc, elem_t val);

elem_t x;
elem_t y;

void *thread_1(void *unused)
{
	mywrite(&x, 1);
	mywrite(&y, 1);
	;
}

void *thread_2(void *unused)
{
	elem_t a = myread(&y);
	elem_t b = myread(&x);
	;
}
