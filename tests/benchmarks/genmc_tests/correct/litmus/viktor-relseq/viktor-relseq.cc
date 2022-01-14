#include "librace.h" 
#include "model-assert.h"
atomic_int lock;

void *threadRa(void *arg)
{
        atomic_fetch_add_explicit(__FILE__, __LINE__, &lock, 1, memory_order_acquire);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &lock, 1, memory_order_relaxed);
        ;
}

void *threadR(void *arg)
{
        atomic_fetch_add_explicit(__FILE__, __LINE__, &lock, 1, memory_order_relaxed);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &lock, 1, memory_order_relaxed);
        ;
}

void *threadRr(void *arg)
{
        atomic_fetch_add_explicit(__FILE__, __LINE__, &lock, 1, memory_order_release);
        ;
}

void *threadRs(void *arg)
{
        atomic_fetch_add_explicit(__FILE__, __LINE__, &lock, 1, memory_order_relaxed);
        ;
}
