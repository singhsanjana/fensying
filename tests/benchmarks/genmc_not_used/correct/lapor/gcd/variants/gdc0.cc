#include "librace.h" 
#include "model-assert.h"
#include <stdlib.h>
#include <stdatomic.h>
#include <threads.h>

#include "../gcd.cc"

int user_user_user_main()
{
	// for testing with small unwinding bounds
	unsigned a_in = 8; // __VERIFIER_nondet_uint(); //=8;
	unsigned b_in = 6; // __VERIFIER_nondet_uint(); //=6;

	assume(a_in > 0);
	assume(b_in > 0);

	start(a_in, b_in);
	/* check_gcd(a_in, b_in, start(a_in, b_in)); */
	return 0;
}
