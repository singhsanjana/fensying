#include "librace.h" 
#include "model-assert.h"
typedef int elem_t;

void myinit(elem_t *loc, elem_t val);
void mywrite(elem_t *loc, elem_t val);
elem_t myread(elem_t *loc);

elem_t a, b, c, d;

void assume(int);

void *thread_1(void *unused)
{
	mywrite(&c, 2);
	mywrite(&a, 2);
	assume(myread(&b) == 2);
	;
}

void *thread_2(void *unused)
{
	mywrite(&c, 1);
	assume(myread(&a) == 2);
	assume(myread(&b) == 1);
	;
}

void *thread_3(void *unused)
{
	mywrite(&b, 2);
	mywrite(&a, 1);
	assume(myread(&c) == 2);;
	;
}

void *thread_4(void *unused)
{
	mywrite(&b, 1);
	assume(myread(&a) == 1);
	assume(myread(&c) == 1);
	;
}
