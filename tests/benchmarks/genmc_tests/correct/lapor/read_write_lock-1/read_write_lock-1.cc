#include "librace.h" 
#include "model-assert.h"
#include <mutex>
/* Testcase from Threader's distribution. For details see:
   http://www.model.in.tum.de/~popeea/research/threader

   This file is adapted from the example introduced in the paper:
   Thread-Modular Verification for Shared-Memory Programs
   by Cormac Flanagan, Stephen Freund, Shaz Qadeer.
*/

#define __VERIFIER_error() MODEL_ASSERT(0)
void assume(int);

int w = 0, r = 0;
atomic_int x, y;

std::mutex atomic_l;

void __VERIFIER_atomic_take_write_lock()
{
	atomic_l.lock();
	assume(w == 0 && r == 0);
	w = 1;
	atomic_l.unlock();
}

void __VERIFIER_atomic_release_write_lock()
{
	atomic_l.lock();
	w = 0;
	atomic_l.unlock();
}

void __VERIFIER_atomic_take_read_lock()
{
	atomic_l.lock();
	assume(w == 0);
	r = r + 1;
	atomic_l.unlock();
}

void __VERIFIER_atomic_release_read_lock()
{
	atomic_l.lock();
	r = r - 1;
	atomic_l.unlock();
}

/* writer */
void *writer(void *arg)
{
	__VERIFIER_atomic_take_write_lock();
	x = 3;
	__VERIFIER_atomic_release_write_lock();
	return 0;
}

/* reader */
void *reader(void *arg)
{
	int l;

	__VERIFIER_atomic_take_read_lock();
	l = x;
	y = l;
	MODEL_ASSERT(y == x);
	__VERIFIER_atomic_release_read_lock();
	return 0;
}
