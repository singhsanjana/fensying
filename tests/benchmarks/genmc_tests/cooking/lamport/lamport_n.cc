#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <threads.h>#include "librace.h" 
#include "model-assert.h"

/* Lamport's fast mutex algorithm 2
 * (https://users.soe.ucsc.edu/~scott/courses/Fall11/221/Papers/Sync/lamport-tocs87.pdf)
 */

#ifndef N
#  warning "N was not defined, assuming 3"
#  define N 3
#endif

void __VERIFIER_assume(intptr_t);
#define await(cond) __VERIFIER_assume(cond)

atomic_intptr_t b[N+1], x, y;

static void lock(intptr_t thread) {
    while (1) {
        b[thread] = true;
        x = thread;
        if (y != 0) {
            b[thread] = false;
            await(y == 0);
	    continue;
	}
        y = thread;
        if (x != thread) {
            b[thread] = false;
            for (intptr_t j = 1; j <= N; j++)
              await(b[j] == false);
            if (y != thread) {
                await(y == 0);
		continue;
	    }
	}
	break;
    }
}

static void unlock(intptr_t thread) {
    y = 0;
    b[thread] = false;
}

static void *thread(void *arg) {
    intptr_t thread = (intptr_t)arg;
    lock(thread);
    unlock(thread);
    return NULL;
}

int main() {
  thrd_t t[N+1];
  for (intptr_t i = 1; i <= N; i++)
    thrd_create(t+i, (thrd_start_t)& thread, NULL);

  for (intptr_t i = 1; i <= N; i++)
	  thrd_join(t[i]);

#ifdef NIDHUGG
  printf("full exec\n");
#endif

  return 0;
}
