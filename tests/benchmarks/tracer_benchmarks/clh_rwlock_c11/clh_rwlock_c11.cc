/*
 * Implemeneted under the RA semantics
 * This benchmark is part of TRACER
 * Get orgiral source code from:
 * https://github.com/pramalhe/ConcurrencyFreaks/blob/master/C11/locks/clh_rwlock.c
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
 * clh Reader-Writer Lock
 *
 * This is a Reader-Writer Lock based on the ideas in the clh Mutex.
 *
 * As far as we can tell, we discovered this algorithm for lock implementation,
 * but if you happen to see some prior art, we would like to know about it, so
 * leave us a comment at http://www.concurrencyfreaks.com
 *
 * Notice that this lock is NOT recursive.
 *
 * This RW lock is "Starvation-Free", assuming that the run-time
 * can provide that kind of guarantee.
 *
 * It is possible to remove the field "mynode" from clh_rwlock_node_t, but
 * then we need to use either a thread-local variable to store it, or we need
 * to return it from clh_rwlock_lock() and pass it to clh_rwlock_unlock()
 * as arguments, which is ok for some usages, but hard to do for others.
 *
 * This locking mechanism has some similarities with the MCS lock because each
 * thread is spinning on the "succ_must_wait" of the previous node, which means that
 * each thread is waiting on a different cache-line, thus reducing
 * synchronization.
 * http://www.cise.ufl.edu/tr/DOC/REP-1992-71.pdf
 *
 */

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"
#include "clh_rwlock.h"
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE  128
#define LOOP 10
#define N 3

// shared variables
int array1[ARRAY_SIZE];
clh_rwlock_t clhrwlock;
thrd_t ts[N];

static clh_rwlock_node_t * clh_rwlock_create_node(char succ_must_wait)
{
    clh_rwlock_node_t * new_node = (clh_rwlock_node_t *)calloc(1, sizeof(clh_rwlock_node_t));
    atomic_store_explicit(__FILE__, __LINE__, &new_node->succ_must_wait, succ_must_wait, memory_order_release);
    return new_node;
}


/*
 * Initializes the rwlock, creating a sentinel node.
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void clh_rwlockx_init(clh_rwlock_t * self)
{
    // We create the first sentinel node unlocked, with succ_must_wait=0
    clh_rwlock_node_t * node = clh_rwlock_create_node(0);
    self->mynode = node;
    atomic_store_explicit(__FILE__, __LINE__, &self->readers_counter, 0, memory_order_release);
    atomic_store_explicit(__FILE__, __LINE__, &self->tail, node, memory_order_release);
}


/*
 * Destroy the rwlock, clearing all memory.
 * You must be sure that there are no other threads currently holding
 * the lock or attempting to.
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void clh_rwlock_destroy(clh_rwlock_t * self)
{
    atomic_store_explicit(__FILE__, __LINE__, &self->readers_counter, 0, memory_order_release);
    free(atomic_load(__FILE__, __LINE__, &self->tail));
}




/*
 * Locks the rwlock for the current thread. Will wait for other threads
 * that did the atomic_exchange() before this one.
 *
 * Progress Condition: Blocking
 */
int clh_rwlock_readlock(clh_rwlock_t * self)
{
    int ok=0;
    // Create the new node locked by default, setting succ_must_wait=1
    clh_rwlock_node_t *mynode = clh_rwlock_create_node(1);
    clh_rwlock_node_t *prev = (clh_rwlock_node_t *)atomic_exchange_explicit(__FILE__, __LINE__, &self->tail, mynode, memory_order_acq_rel);

    // This thread's node is now in the queue, so wait until it is its turn
    char prev_islocked = atomic_load_explicit(__FILE__, __LINE__, &prev->succ_must_wait, memory_order_acquire);
    if (prev_islocked) {
        for (int i=0; i<LOOP; i++) {
          if (!prev_islocked) {
            ok=1;
            break;
          }
        }
        prev_islocked = atomic_load_explicit(__FILE__, __LINE__, &prev->succ_must_wait, memory_order_acquire);
    }

    if (ok==0) return 0;

    // Incrementing the readers_counter will prevent a Writer from going in
    atomic_fetch_add_explicit(__FILE__, __LINE__, &self->readers_counter, 1, memory_order_acq_rel);

    // This will allow the next thread to go in, but only if it is a Reader
    atomic_store_explicit(__FILE__, __LINE__, &mynode->succ_must_wait, 0, memory_order_release);

    // This thread has acquired the lock and it is now safe to
    // cleanup the memory of the previous node.
    free(prev);
    return 1;
}

/*
 *
 * Progress Condition: Lock-Free, or Wait-Free Population Oblivious (on x86)
 */
void clh_rwlock_readunlock(clh_rwlock_t * self)
{
    long prev_counter = atomic_fetch_add_explicit(__FILE__, __LINE__, &self->readers_counter, -1, memory_order_acq_rel);
    if (prev_counter == 0) {
        return;
    }
}


/*
 * Locks the rwlock for the current thread. Will wait for other threads
 * that did the atomic_exchange() before this one.
 *
 * Progress Condition: Blocking
 */
int clh_rwlock_writelock(clh_rwlock_t * self)
{
    int ok=0;
    // Create the new node locked by default, setting succ_must_wait=1
    clh_rwlock_node_t *mynode = clh_rwlock_create_node(1);
    clh_rwlock_node_t *prev = (clh_rwlock_node_t *)atomic_exchange_explicit(__FILE__, __LINE__, &self->tail, mynode, memory_order_acq_rel);

    // This thread's node is now in the queue, so wait until it is its turn
    char prev_islocked = atomic_load_explicit(__FILE__, __LINE__, &prev->succ_must_wait, memory_order_acquire);
    if (prev_islocked) {
        for (int i=0; i<LOOP; i++) {
          if (!prev_islocked) {
            ok=1;
            break;
          }
          prev_islocked = atomic_load_explicit(__FILE__, __LINE__, &prev->succ_must_wait, memory_order_acquire);
        }
    }

    if (ok==0) return 0;

    ok=0;
    // Even though succ_must_wait is 0, there may be unfinished Readers, so spin/wait
    // until they're over.
    long readers_counter = atomic_load_explicit(__FILE__, __LINE__, &self->readers_counter, memory_order_acquire);
    if (readers_counter != 0) {
        for (int i=0; i<LOOP; i++) {
          if (readers_counter == 0) {
            ok=1;
            break;
          }
          readers_counter = atomic_load_explicit(__FILE__, __LINE__, &self->readers_counter, memory_order_acquire);
        }
    }
    if (ok==0) return 0;

    // This thread has acquired the lock
    self->mynode = mynode;
    free(prev);
    return 1;
}

/*
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void clh_rwlock_writeunlock(clh_rwlock_t * self)
{
    atomic_store_explicit(__FILE__, __LINE__, &self->mynode->succ_must_wait, 0, memory_order_release);
}


void worker_thread(void *arg)
{
  	int tid = *((int *)arg);  	
    int ncarray[ARRAY_SIZE*10];

    for (int i = 0; i < ARRAY_SIZE*10; i++) ncarray[i] = 99;

    /* Critical path for ticket_mutex_t */
    if (clh_rwlock_readlock(&clhrwlock)==1) {
        for (int i = 1; i < ARRAY_SIZE; i++) {
            if (array1[i] != array1[0]) MODEL_ASSERT(0);
        }
        clh_rwlock_readunlock(&clhrwlock);
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
    clh_rwlockx_init(&clhrwlock);

    for (int i=0; i<N; i++) {
        arg[i] = i;
        thrd_create(&ts[i], worker_thread, &arg[i]);
    }
  
  	for (int i=0; i<N; i++) {
  		thrd_join(ts[i]);
  	}
  
    return 0;
}
