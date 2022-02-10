/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Adapted from: https://github.com/sosy-lab/sv-benchmarks/blob/master/c/pthread/queue_ok_true-unreach-call.c */

#include <threads.h>
#include <stdatomic.h>
#include <mutex>
#include "librace.h"
#include "model-assert.h"

#define SIZE	(12)
#define EMPTY	(-1)
#define FULL	(-2)
#define FALSE	(0)
#define TRUE	(1)
#define NUM_ENQUEUE_THREADS 5
#define NUM_DEQUEUE_THREADS 4

atomic_int element[SIZE];
atomic_int head;
atomic_int tail;
atomic_int amount;
std::mutex  *lock;
atomic_int stored_elements[SIZE];
atomic_int enqueue_flag, dequeue_flag;

void init(void) 
{
  atomic_init(&head, 0);
  atomic_init(&tail, 0);
  atomic_init(&amount, 0);
  for (int i=0; i<SIZE; i++) {
    atomic_init(&element[i], 0);
  }
}

int empty(void) 
{
  int _head, _tail, _amount;
  _head = atomic_load_explicit(__FILE__, __LINE__, &head, memory_order_acquire);
  _tail = atomic_load_explicit(__FILE__, __LINE__, &tail, memory_order_acquire);
  _amount = atomic_load_explicit(__FILE__, __LINE__, &amount, memory_order_acquire);
  
  if (_head == _tail && _amount==0) 
  { 
    return EMPTY;
  }
  else 
    return 0;
}

int full(void) 
{
  int _amount;
  _amount = atomic_load_explicit(__FILE__, __LINE__, &amount, memory_order_acquire);
  if (_amount == SIZE) 
  {  
    return FULL;
  } 
  else
    return 0;
}

int enqueue(int x) 
{
  int _tail, _amount;
  _tail = atomic_load_explicit(__FILE__, __LINE__, &tail, memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &element[_tail], x, memory_order_release);
  
  _amount = atomic_load_explicit(__FILE__, __LINE__, &amount, memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &amount, _amount+1, memory_order_release);
  
  _tail = atomic_load_explicit(__FILE__, __LINE__, &tail, memory_order_acquire);
  if (_tail == SIZE-1) 
  {
    atomic_store_explicit(__FILE__, __LINE__, &tail, 0, memory_order_release);
  } else {
   _tail = atomic_load_explicit(__FILE__, __LINE__, &tail, memory_order_acquire);
   atomic_store_explicit(__FILE__, __LINE__, &tail, _tail+1, memory_order_release);
 }

 return 0;
}


int dequeue() 
{
  int x, _head, _amount;
  
  _head = atomic_load_explicit(__FILE__, __LINE__, &head, memory_order_acquire);
  x = atomic_load_explicit(__FILE__, __LINE__, &element[_head], memory_order_acquire);

  _amount = atomic_load_explicit(__FILE__, __LINE__, &amount, memory_order_acquire);
  atomic_store_explicit(__FILE__, __LINE__, &amount, _amount-1, memory_order_release);

  _head = atomic_load_explicit(__FILE__, __LINE__, &head, memory_order_acquire);
  if (_head == SIZE-1) 
  {
    atomic_store_explicit(__FILE__, __LINE__, &head, 0, memory_order_release);
  } 
  else {
  	_head = atomic_load_explicit(__FILE__, __LINE__, &head, memory_order_acquire);
    atomic_store_explicit(__FILE__, __LINE__, &head, _head+1, memory_order_release);
  }
  return x;
}

void enqueuethread(void *arg)
{
	int value, i, _enqueue_flag;
  int enqueue_number = 0;
  value = *((int *)arg);

  lock->lock();
  _enqueue_flag = atomic_load_explicit(__FILE__, __LINE__, &enqueue_flag, memory_order_acquire);
  if (_enqueue_flag)
  {
    for(i=0; i<SIZE; i++)  
    {
      if (full() != FULL) {
        value += enqueue_number;
        enqueue_number++;
        enqueue(value);
        atomic_store_explicit(__FILE__, __LINE__, &stored_elements[i], value, memory_order_release);
        if (empty()) MODEL_ASSERT(0);
      }
    }
    atomic_store_explicit(__FILE__, __LINE__, &enqueue_flag, FALSE, memory_order_release);
    atomic_store_explicit(__FILE__, __LINE__, &dequeue_flag, TRUE, memory_order_release);
  }
  lock->unlock();

  return;
}


void dequeuethread(void *arg)
{
  int i, _dequeue_flag, _stored_elements_i;

  lock->lock();
  _dequeue_flag = atomic_load_explicit(__FILE__, __LINE__, &dequeue_flag, memory_order_acquire);
  if (_dequeue_flag)
  {
    for(i=0; i<SIZE; i++)  
    {
      if (empty()!=EMPTY) {
        _stored_elements_i = atomic_load_explicit(__FILE__, __LINE__, &stored_elements[i], memory_order_acquire);
        if (!(dequeue() == _stored_elements_i)) {
          MODEL_ASSERT(0);
        }
      }
    }
    atomic_store_explicit(__FILE__, __LINE__, &dequeue_flag, FALSE, memory_order_release);
    atomic_store_explicit(__FILE__, __LINE__, &enqueue_flag, TRUE, memory_order_release);
  }
  lock->unlock();
  return;
}



int user_main(int argc, char **argv)
{
	int i, arg[NUM_ENQUEUE_THREADS];
	thrd_t t1s[NUM_ENQUEUE_THREADS], t2s[NUM_DEQUEUE_THREADS];

	atomic_init(&enqueue_flag, TRUE);
	atomic_init(&dequeue_flag, FALSE);
	lock = new std::mutex();

	init();
	
  if (!(empty()==EMPTY)) {
    MODEL_ASSERT(0);
  }

  for (i=0; i<SIZE; i++) {
    atomic_init(&stored_elements[i], 0);
  }

  for (i=0; i<NUM_ENQUEUE_THREADS; i++) {
    arg[i] = i;
    thrd_create(&t1s[i], enqueuethread, &arg[i]);
  }

  for (i=0; i<NUM_DEQUEUE_THREADS; i++) {
    thrd_create(&t2s[i], dequeuethread, NULL);
  }

  return 0;
}
