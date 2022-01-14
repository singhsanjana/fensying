#include "librace.h" 
#include "model-assert.h"
#include <mutex>

// Copyright (c) 2015 Michael Tautschnig <michael.tautschnig@qmul.ac.uk>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


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

#define __VERIFIER_error(void) MODEL_ASSERT(0)
void assume(int);

extern unsigned int __VERIFIER_nondet_uint();

void MODEL_ASSERT(int cond)
{
	if (!(cond))
		__VERIFIER_error();
	return;
}

atomic_uint a, b;

std::mutex lock;

void __VERIFIER_atomic_dec_a()
{
	lock.lock();
	if (a > b)
		a = a - b;
	lock.unlock();
}

void __VERIFIER_atomic_dec_b()
{
	lock.lock();
	if (b > a)
		b = b - a;
	lock.unlock();
}

void *dec_a(void* arg)
{
	while (a != b) {
		__VERIFIER_atomic_dec_a();
	}

	;
}

void *dec_b(void* arg)
{
	while (a != b) {
		__VERIFIER_atomic_dec_b();
	}

	;
}

unsigned start(unsigned a_in, unsigned b_in)
{
	thrd_t t1, t2;

	a = a_in;
	b = b_in;

	if (thrd_create(&t1, (thrd_start_t)& dec_a, NULL))
		MODEL_ASSERT(0);
	if (thrd_create(&t2, (thrd_start_t)& dec_b, NULL))
		MODEL_ASSERT(0);

	if (thrd_join(t1))
		MODEL_ASSERT(0);
	if (thrd_join(t2))
		MODEL_ASSERT(0);

	return a;
}

void check_gcd(unsigned a_in, unsigned b_in, unsigned gcd)
{
	unsigned guessed_gcd = 5; //__VERIFIER_nondet_uint();
	assume(guessed_gcd > 1);
	assume(a_in % guessed_gcd == 0);
	assume(b_in % guessed_gcd == 0);

	MODEL_ASSERT(a_in % gcd == 0);
	MODEL_ASSERT(b_in % gcd == 0);

	MODEL_ASSERT(gcd >= guessed_gcd);
}
