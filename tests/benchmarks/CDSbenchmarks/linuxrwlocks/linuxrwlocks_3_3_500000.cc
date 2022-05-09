#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#include "librace.h"
#include "model-assert.h"


#define writer_LOOP 3
#define reader_LOOP 3
#define LOOP 500000

#define RW_LOCK_BIAS            0x00100000
#define WRITE_LOCK_CMP          RW_LOCK_BIAS

/** Example implementation of linux rw lock along with 2 thread test
 *  driver... */

typedef union {
	atomic_int lock;
} rwlock_t;

static inline int read_can_lock(rwlock_t *lock)
{
	return atomic_load_explicit(__FILE__, __LINE__, &lock->lock, memory_order_seq_cst) > 0;
}

static inline int write_can_lock(rwlock_t *lock)
{
	return atomic_load_explicit(__FILE__, __LINE__, &lock->lock, memory_order_seq_cst) == RW_LOCK_BIAS;
}

static inline void read_lock(rwlock_t *rw)
{
	// int priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_seq_cst);
	int priorvalue = atomic_load_explicit(__FILE__, __LINE__, &rw->lock, memory_order_seq_cst);
	atomic_store_explicit(__FILE__, __LINE__, &rw->lock, priorvalue-1, memory_order_relaxed);
	for (int i = 0; i < LOOP; i++) {
		if (priorvalue <= 0) {
			atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_seq_cst);
			for (int j = 0; j < LOOP; j++) {
				if (atomic_load_explicit(__FILE__, __LINE__, &rw->lock, memory_order_seq_cst) <= 0) {
					// thrd_yield();
				}
				else {
					priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_seq_cst);
					break;
				}
			}
			break;
		}
	}
}

static inline void write_lock(rwlock_t *rw)
{
	// int priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_seq_cst);
	int priorvalue = atomic_load_explicit(__FILE__, __LINE__, &rw->lock, memory_order_relaxed);
	atomic_store_explicit(__FILE__, __LINE__, &rw->lock, priorvalue-RW_LOCK_BIAS, memory_order_seq_cst);
	for (int i = 0; i < LOOP; i++) {
		if (priorvalue != RW_LOCK_BIAS) {
			atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_seq_cst);
			for (int j = 0; j < LOOP; j++) {
				if (atomic_load_explicit(__FILE__, __LINE__, &rw->lock, memory_order_seq_cst) != RW_LOCK_BIAS) {
					// thrd_yield();
				}
				else {
					priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_seq_cst);
					break;
				}
			}
			break;
		}
	}
}

static inline int read_trylock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_acquire);
	if (priorvalue > 0)
		return 1;

	atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_seq_cst);
	return 0;
}

static inline int write_trylock(rwlock_t *rw)
{
	int priorvalue = atomic_fetch_sub_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_acquire);
	if (priorvalue == RW_LOCK_BIAS)
		return 1;

	atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_seq_cst);
	return 0;
}

static inline void read_unlock(rwlock_t *rw)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, 1, memory_order_seq_cst);
}

static inline void write_unlock(rwlock_t *rw)
{
	atomic_fetch_add_explicit(__FILE__, __LINE__, &rw->lock, RW_LOCK_BIAS, memory_order_seq_cst);
}

rwlock_t mylock;
// int shareddata1, shareddata2;
atomic_int shareddata1, shareddata2;

static void fn1(void *obj)
{
	int i;
	for(i = 0; i < reader_LOOP; i++) {
		if ((i % 2) == 0) {
			read_lock(&mylock);
			// int a = load_32(&shareddata2);
			// int b = load_32(&shareddata1);
			int a = atomic_load_explicit(__FILE__, __LINE__, &shareddata2, memory_order_relaxed);
			int b = atomic_load_explicit(__FILE__, __LINE__, &shareddata1, memory_order_relaxed);
			read_unlock(&mylock);
			MODEL_ASSERT(a==0 || b==1);
		} else {
			// write_lock(&mylock);
			// store_32(&shareddata,(unsigned int)i);
			// write_unlock(&mylock);
		}
	}
}

static void fn2(void *obj)
{
	int i;
	for(i = 0; i < writer_LOOP; i++) {
		if ((i % 2) == 0) {
			// read_lock(&mylock);
			// b = load_32(&shareddata);
			// read_unlock(&mylock);
		} else {
			write_lock(&mylock);
			// store_32(&shareddata1,1);
			// store_32(&shareddata2,1);
			atomic_store_explicit(__FILE__, __LINE__, &shareddata1, 1, memory_order_relaxed);
			atomic_store_explicit(__FILE__, __LINE__, &shareddata2, 1, memory_order_relaxed);
			write_unlock(&mylock);
		}
	}
}

int user_main(int argc, char **argv)
{
	thrd_t t1, t2;
	atomic_init(&mylock.lock, RW_LOCK_BIAS);
	atomic_init(&shareddata1, 0);
	atomic_init(&shareddata2, 0);
	// shareddata1 = 0;
	// shareddata2 = 0;

	thrd_create(&t1, (thrd_start_t)&fn1, NULL);
	thrd_create(&t2, (thrd_start_t)&fn2, NULL);

	thrd_join(t1);
	thrd_join(t2);

	return 0;
}
