#include "librace.h" 
#include "model-assert.h"
#include <mutex>
void assume(int);

atomic_int x;
atomic_int y;
atomic_int z;

std::mutex l;

void *thread_1(void *unused)
{
	l.lock();
	y = 1;
	l.unlock();

	assume(x == 2);
	;
}

void *thread_2(void *unused)
{
	l.lock();
	x = 1;
	assume(y == 0);
	l.unlock();
	;
}

void *thread_3(void *unused)
{
	l.lock();
	x = 2;
	assume(z == 0);
	l.unlock();
	;
}

void *thread_4(void *unused)
{
	l.lock();
	z = 1;
	l.unlock();

	assume(x == 1);
	;
}
