#include "librace.h" 
#include "model-assert.h"
atomic_int a;
atomic_int b;
atomic_int c;
atomic_int d;

void assume(int);

void *thread_1(void *unused)
{
	c = 2;
	a = 2;
	assume(b == 2);
	;
}

void *thread_2(void *unused)
{
	c = 1;
	assume(a == 2);
	assume(b == 1);
	;
}

void *thread_3(void *unused)
{
	b = 2;
	a = 1;
	assume(c == 2);;
	;
}

void *thread_4(void *unused)
{
	b = 1;
	assume(a == 1);
	assume(c == 1);
	;
}
