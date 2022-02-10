/* Copyright (C) 2018 Tuan Phong Ngo 
 * This benchmark is part of TRACER
 */

/* Adapted from: https://github.com/sosy-lab/sv-benchmarks/blob/master/c/pthread-atomic/gcd_true-unreach-call_true-termination.c */

/*
VerifyThis ETAPS 2015, Challenge 2

PARALLEL GCD (60 minutes)
=========================

Algorithm description
---------------------

Various parallel GCD algorithms exist. In this challenge, we consider a
simple Euclid-like algorithm with two parallel threads. One thread
subtracts in one direction, the other thread subtracts in the other
direction, and eventually this procedure converges on GCD.


Implementation
--------------

In pseudocode, the algorithm is described as follows:

(
  WHILE a != b DO                                        
      IF a>b THEN a:=a-b ELSE SKIP FI
  OD
||
  WHILE a != b DO                                        
       IF b>a THEN b:=b-a ELSE SKIP FI
  OD
);
OUTPUT a


Verification tasks
------------------

Specify and verify the following behaviour of this parallel GCD algorithm:

Input:  two positive integers a and b
Output: a positive number that is the greatest common divisor of a and b

Feel free to add synchronisation where appropriate, but try to avoid
blocking of the parallel threads.


Sequentialization
-----------------

If your tool does not support reasoning about parallel threads, you may
verify the following pseudocode algorithm:

WHILE a != b DO
    CHOOSE(
         IF a > b THEN a := a - b ELSE SKIP FI,
         IF b > a THEN b := b - a ELSE SKIP FI
    )
OD;
OUTPUT a
*/

#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include <mutex>
#include "model-assert.h"

#define A 17
#define B 7
#define LOOP 8

// shared variables
atomic_int a, b; 
std::mutex  *lock;


void atomic_dec_a(void)
{
  int _a, _b;

  lock->lock();
  _a = atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_acquire);
  _b = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_acquire);
  if(_a > _b) {
   _a = atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_acquire);
   _b = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_acquire);
   atomic_store_explicit(__FILE__, __LINE__, &a, _a - _b, memory_order_release);
 }
 lock->unlock();

}


void atomic_dec_b(void)
{
  int _a, _b;

  lock->lock();
  _a = atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_acquire);
  _b = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_acquire);
  if(_b > _a) {
   _a = atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_acquire);
   _b = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_acquire);
   atomic_store_explicit(__FILE__, __LINE__, &b, _b - _a, memory_order_release);
 }
 lock->unlock();

}


void  dec_a(void* arg)
{
  int _a, _b;
  for (int i=0; i<LOOP; i++)
  {
   _a = atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_acquire);
   _b = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_acquire);

  	if (_a != _b) // using for-if to have finite runs
  	{
      atomic_dec_a();
      _a = atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_acquire);
      _b = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_acquire);
      MODEL_ASSERT(_a >= 0 && _b>= 0);
    }
  }

}


void  dec_b(void* arg)
{
  int _a, _b;

  for (int i=0; i<LOOP; i++)
  {
   _a = atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_acquire);
   _b = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_acquire);

  	if (_a != _b) // using for-if to have finite runs
  	{
      atomic_dec_b();
      _a = atomic_load_explicit(__FILE__, __LINE__, &a, memory_order_acquire);
      _b = atomic_load_explicit(__FILE__, __LINE__, &b, memory_order_acquire);
      MODEL_ASSERT(_a >= 0 && _b >= 0);
    }
  }

}


int user_main(int argc, char **argv)
{
	int _a, _b;

	thrd_t t1, t2;
	
	atomic_init(&a, A);
	atomic_init(&b, B);

	lock = new std::mutex();


	thrd_create(&t1, dec_a, NULL);
	thrd_create(&t2, dec_b, NULL);
 

	return 0;
}
