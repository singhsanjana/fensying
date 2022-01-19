#include "librace.h" 
#include "model-assert.h"
#include <mutex>
/* Testcase from Threader's distribution. For details see:
   http://www.model.in.tum.de/~popeea/research/threader
   This file is adapted from the example introduced in the paper:
   Thread-Modular Verification for Shared-Memory Programs
   by Cormac Flanagan, Stephen Freund, Shaz Qadeer.
*/

void assume(int);
#define __VERIFIER_error() MODEL_ASSERT(0)

/* #define MODEL_ASSERT(e) if (!(e)) ERROR: __VERIFIER_error() */

int block;
int busy; // boolean flag indicating whether the block has been allocated to an inode
int inode;
std::mutex m_inode; // protects the inode
std::mutex m_busy; // protects the busy flag

void *allocator(void *_)
{
	m_inode.lock();
	if (inode == 0) {
		m_busy.lock();
		busy = 1;
		m_busy.unlock();
		inode = 1;
	}
	block = 1;
	MODEL_ASSERT(block == 1);
	m_inode.unlock();
	;
}

void *de_allocator(void *_)
{
	m_busy.lock();
	if (busy == 0) {
		block = 0;
		MODEL_ASSERT(block == 0);
	}
	m_busy.unlock();
	return ((void *)0);
}
