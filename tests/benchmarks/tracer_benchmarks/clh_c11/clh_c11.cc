/*
 * Implemeneted under the RA semantics
 * This benchmark is part of TRACER
 * Get orgiral source code from:
 * https://github.com/pramalhe/ConcurrencyFreaks/blob/master/C11/locks/clh_mutex.c
 */

/******************************************************************************
 * Copyright (c) 2014, Pedro Ramalhete, Andreia Correia
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Concurrency Freaks nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
 */

/*
 * This is a C11 implementation of the CLH lock where the previous node is removed.
 * See slide 45 here:
 * http://www.cs.rice.edu/~vs3/comp422/lecture-notes/comp422-lec19-s08-v1.pdf
 *
 * Notice that this lock is NOT recursive, but it is easy to make it recursive.
 *
 * This mutual exclusion lock is "Starvation-Free", assuming that the run-time
 * can provide that kind of guarantee.
 *
 * It is possible to remove the field "mynode" from clh_mutex_node_t, but
 * then we need to use either a thread-local variables to store them, or we
 * need to return it from clh_mutex_lock() and pass it to
 * clh_mutex_unlock() as argument, which is ok for some usages, but hard to
 * do for others.
 * In C++1x, it should be easy to use this technique with a "Guard" pattern
 * and then pass the operation on the object associated with the mutex through
 * a lambda.
 * The advantage of this last approach is that there are no loads, stores, or
 * false-sharing for "mynode"  because the compiler can put those variables on
 * a register, or at worse, on the current thread's stack.
 *
 * This locking mechanism has some similarities with the MCS lock because each
 * thread is spinning on the "islocked" of the previous node, which means that
 * each thread is waiting on a different cache-line, thus reducing
 * synchronization.
 * http://www.cise.ufl.edu/tr/DOC/REP-1992-71.pdf
 * The main different from the MCS lock (section 2) is that MCS may have to do
 * an Compare-And-Swap in addition to the atomic_store() in the unlock function.
 *
 * The CLH lock uses the (theoretically) minimum amount of synchronization
 * operations to perform a mutual exclusion lock, because:
 * - In the lock() function it has a single synchronized operation
 *   atomic_exchange() with one acquire and one release barrier.
 * - In the unlock() function is has a single synchronized operation, the
 *   atomic_store() with a release barrier.
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
#include "clh_mutex.h"
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE  128
#define LOOP 10
#define N 3

// shared variables
int array1[ARRAY_SIZE];
clh_mutex_t clhmutex;
thrd_t ts[N];

static clh_mutex_node_t * clh_mutex_create_node(char islocked)
{
  clh_mutex_node_t * new_node = (clh_mutex_node_t *)calloc(1, sizeof(clh_mutex_node_t));
  atomic_store_explicit(__FILE__, __LINE__, &new_node->succ_must_wait, islocked, memory_order_release);
  return new_node;
}


/*
 * Initializes the mutex, creating a sentinel node.
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void clh_mutex_init(clh_mutex_t * self)
{
  // We create the first sentinel node unlocked, with islocked=0
  clh_mutex_node_t * node = clh_mutex_create_node(0);
  self->mynode = node;
  atomic_store_explicit(__FILE__, __LINE__, &self->tail, node, memory_order_release);
}


/*
 * Destroy the mutex, clearing all memory.
 * You must be sure that there are no other threads currently holding
 * the lock or attempting to.
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void clh_mutex_destroy(clh_mutex_t * self)
{
  free(atomic_load(__FILE__, __LINE__, &self->tail));
}




/*
 * Locks the mutex for the current thread. Will wait for other threads
 * that did the atomic_exchange() before this one.
 *
 * Progress Condition: Blocking
 */
int clh_mutex_lock(clh_mutex_t * self)
{
  int ok = 0;
  // Create the new node locked by default, setting islocked=1
  clh_mutex_node_t *mynode = clh_mutex_create_node(1);
  clh_mutex_node_t *prev = (clh_mutex_node_t *)atomic_exchange_explicit(__FILE__, __LINE__, &self->tail, mynode, memory_order_acq_rel);
  
  // This thread's node is now in the queue, so wait until it is its turn
  char prev_islocked = atomic_load_explicit(__FILE__, __LINE__, &prev->succ_must_wait, memory_order_acquire);
  if (prev_islocked) {
    for (int i=0; i<LOOP; i++) {
      if (!prev_islocked) {
        ok = 1;
        break;
      }
      prev_islocked = atomic_load_explicit(__FILE__, __LINE__, &prev->succ_must_wait, memory_order_acquire);
    }
    if (ok==0) return 0;
  }
  
  // This thread has acquired the lock on the mutex and it is now safe to
  // cleanup the memory of the previous node.
  free(prev);
  
  // Store mynode for clh_mutex_unlock() to use. We could replace
  // this with a thread-local, not sure which is faster.
  self->mynode = mynode;
  
  return 1;
}

/*
 * Unlocks the mutex. Assumes that the current thread holds the lock on the
 * mutex.
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void clh_mutex_unlock(clh_mutex_t * self)
{
  // We assume that if this function was called, it is because this thread is
  // currently holding the lock, which means that self->mynode is pointing to
  // the current thread's mynode.
  if (self->mynode == NULL) {
    // ERROR: This will occur if unlock() is called without a lock()
    return;
  }
  atomic_store_explicit(__FILE__, __LINE__, &self->mynode->succ_must_wait, 0, memory_order_release);
}


void worker_thread(void *arg)
{
  	int tid = *((int *)arg);  	
    int ncarray[ARRAY_SIZE*10];

    for (int i = 0; i < ARRAY_SIZE*10; i++) ncarray[i] = 99;

    /* Critical path for ticket_mutex_t */
    if (clh_mutex_lock(&clhmutex)==1) {
        for (int i = 1; i < ARRAY_SIZE; i++) {
            if (array1[i] != array1[0]) MODEL_ASSERT(0);
        }
        clh_mutex_unlock(&clhmutex);
    }
    
    // Non-critical path, about 10x larger
    for (int i = 1; i < ARRAY_SIZE*10; i++) {
        if (ncarray[i] != ncarray[0]) MODEL_ASSERT(0);
    }
}

int user_main(int argc, char **argv)
{  	
  	int arg[N];

    for (int i = 0; i < ARRAY_SIZE; i++) array1[i] = 0;
    clh_mutex_init(&clhmutex);

    for (int i=0; i<N; i++) {
        arg[i] = i;
        thrd_create(&ts[i], worker_thread, &arg[i]);
    }
  
  	for (int i=0; i<N; i++) {
  		thrd_join(ts[i]);
  	}
  
    return 0;
}
