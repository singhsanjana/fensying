/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* The original benchmark is from CDSChecker */

/* Adapted to test with RCMC and TRACER */

#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"

#define LOOP 3
#define OUTER_LOOP 2
#define LB LOOP*OUTER_LOOP

#define RW_LOCK_BIAS            0x00100000
#define WRITE_LOCK_CMP          RW_LOCK_BIAS

/** Example implementation of linux rw lock along with 2 thread test
 *  driver... */

typedef union {
	atomic_int lock;
} rwlock_t;

static inline int read_can_lock(rwlock_t *lock)
{
	return atomic_load_explicit(__FILE__, __LINE__, &lock->lock, memory_order_acquire) > 0;
}

static inline int write_can_lock(rwlock_t *lock)
{
	return atomic_load_explicit(__FILE__, __LINE__, &lock->lock, memory_order_acquire) == RW_LOCK_BIAS;
}

static inline void read_lock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_acq_rel);
	int OK = 0;
	for (int i=0; i<OUTER_LOOP; i++) {
           if (priorvalue <= 0) {
				atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_acq_rel);
				OK = 0;		
				for (int j=0; j<LOOP; j++) {
		      		if (atomic_load_explicit(__FILE__, __LINE__, &rw->lock, memory_order_acquire) <= 0) {
		        		//thrd_yield();
		      		} else {
		        		OK = 1;
		        		break;
		      		}
				}
				if (OK == 1)
					priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_acq_rel);
		    	} else
		      		break;
			}
}

static inline void write_lock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_acq_rel);
	int OK = 0;
	for (int i=0; i<OUTER_LOOP; i++) {
	   if (priorvalue != RW_LOCK_BIAS) {
        	atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_acq_rel);
        	OK = 0;
        	for (int j=0; j<LOOP; j++) {
           		if (atomic_load_explicit(__FILE__, __LINE__, &rw->lock, memory_order_acquire) != RW_LOCK_BIAS) {
             		//thrd_yield();
           		} else {
             		OK = 1;
             		break;
           		}
        	}
        	if (OK==1)
          		priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_acq_rel);
	   } else
		break;
	}
}

static inline int read_trylock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_acq_rel);
	if (priorvalue > 0)
		return 1;

	atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_acq_rel);
	return 0;
}

static inline int write_trylock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_acq_rel);
	if (priorvalue == RW_LOCK_BIAS)
		return 1;

	atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_acq_rel);
	return 0;
}

static inline void read_unlock(rwlock_t *rw)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_acq_rel);
}

static inline void write_unlock(rwlock_t *rw)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_acq_rel);
}

rwlock_t mylock;

void thread_reader(void *unused)
{
	read_lock(&mylock);
	read_unlock(&mylock);
}

void thread_writer(void *unused)
{
	write_lock(&mylock);
	write_unlock(&mylock);
}

int user_main(int argc, char **argv)
{	
	thrd_t t1, t2;
	atomic_init(&mylock.lock, RW_LOCK_BIAS);

	thrd_create(&t1, thread_reader, NULL);
	thrd_create(&t2, thread_writer, NULL);

	return 0;
}
