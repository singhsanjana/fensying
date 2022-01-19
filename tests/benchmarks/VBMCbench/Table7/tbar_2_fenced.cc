#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 2 


atomic_int childnotready;
atomic_int parentsense;
atomic_int tdiff0 , tdiff1;
atomic_int __fence_var;

void t0(void *)
{
    int rsense = 1;
    int rchildnotready = -1;
    int rtdiff1 = -1;
    for(int i=0;i<LOOP;i++)
    {
        atomic_store_explicit(__FILE__, __LINE__, &tdiff0, 1, memory_order_release);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rchildnotready = atomic_load_explicit(__FILE__, __LINE__, &childnotready, memory_order_acquire);
        if(!(rchildnotready==0))return;
        atomic_store_explicit(__FILE__, __LINE__, &childnotready, 1, memory_order_release);
        atomic_store_explicit(__FILE__, __LINE__, &parentsense, rsense, memory_order_release);
        rsense = 1 - rsense;
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rtdiff1 = atomic_load_explicit(__FILE__, __LINE__, &tdiff1, memory_order_acquire);
        MODEL_ASSERT(rtdiff1!=0);
    }
}



void t1(void *)
{
    int rsense = 1;
    int rtdiff0 = -1;
    int rparentsense = -1;
    for(int i=0;i<LOOP;i++)
    {
        atomic_store_explicit(__FILE__, __LINE__, &tdiff1, 1, memory_order_release);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        atomic_store_explicit(__FILE__, __LINE__, &childnotready, 0, memory_order_release);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rparentsense = atomic_load_explicit(__FILE__, __LINE__, &parentsense, memory_order_acquire);
        if(!(rparentsense==rsense))return;
        rsense = 1 - rsense;
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rtdiff0 = atomic_load_explicit(__FILE__, __LINE__, &tdiff0, memory_order_acquire);
        MODEL_ASSERT(rtdiff0!=0);
    }
}

int *propertyChecking(void* arg)
{
    return 0;
}

int user_main(int argc, char **argv)
{
    thrd_t _t1, _t2;
    atomic_init(&childnotready , 1);
    atomic_init(&parentsense , 0);
    atomic_init(&tdiff0 , 0);
    atomic_init(&tdiff1 , 0);
    atomic_init(&__fence_var, 0);
    thrd_create(&_t1, t0, 0);
    thrd_create(&_t2, t1, 0);
    return 0;
}



