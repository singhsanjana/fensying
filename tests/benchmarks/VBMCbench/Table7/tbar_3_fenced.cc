#include <threads.h>
#include <stdatomic.h>
#include "librace.h"
#include "model-assert.h"

#define LOOP 2 

atomic_int childnotready0_0,  childnotready0_1 ;
atomic_int parentsense1, parentsense2 ;
atomic_int tdiff0, tdiff1 , tdiff2 ;
atomic_int __fence_var;

void t0(void *)
{
    int rsense = 1;
    int rchildnotready0_0 = -1;
    int rchildnotready0_1 = -1;
    int rtdiff1 = -1;
    int rtdiff2 = -1;
    for(int i=0;i<LOOP;i++)
    {
        atomic_store_explicit(__FILE__, __LINE__, &tdiff0, 1, memory_order_release);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rchildnotready0_0 = atomic_load_explicit(__FILE__, __LINE__, &childnotready0_0, memory_order_acquire);
        if(!(rchildnotready0_0 ==0)) return;
        rchildnotready0_1 = atomic_load_explicit(__FILE__, __LINE__, &childnotready0_1, memory_order_acquire);
        if(!(rchildnotready0_1==0)) return;
        atomic_store_explicit(__FILE__, __LINE__, &childnotready0_0, 1, memory_order_release);
        atomic_store_explicit(__FILE__, __LINE__, &childnotready0_1, 1, memory_order_release);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        atomic_store_explicit(__FILE__, __LINE__, &parentsense1, rsense, memory_order_release);
        atomic_store_explicit(__FILE__, __LINE__, &parentsense2, rsense, memory_order_release);
        rsense = 1 - rsense;
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rtdiff1 = atomic_load_explicit(__FILE__, __LINE__, &tdiff1, memory_order_acquire);
        rtdiff2 = atomic_load_explicit(__FILE__, __LINE__, &tdiff2, memory_order_acquire);
        MODEL_ASSERT(rtdiff1 != 0 && rtdiff2 != 0);
    }
}



void t1(void *)
{
    int rsense = 1;
    int rtdiff0 = -1;
    int rtdiff2 = -1;
    int rparentsense1 = -1;
    for(int i=0;i<LOOP;i++)
    {
        atomic_store_explicit(__FILE__, __LINE__, &tdiff1, 1, memory_order_release);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        atomic_store_explicit(__FILE__, __LINE__, &childnotready0_0, 0, memory_order_release);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rparentsense1 = atomic_load_explicit(__FILE__, __LINE__, &parentsense1, memory_order_acquire);
        if(!(rparentsense1==rsense)) return;
        rsense = 1 - rsense;
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rtdiff0 = atomic_load_explicit(__FILE__, __LINE__, &tdiff0, memory_order_acquire);
        rtdiff2 = atomic_load_explicit(__FILE__, __LINE__, &tdiff2, memory_order_acquire);
        MODEL_ASSERT(rtdiff0!=0 && rtdiff2 !=0);
    }
}

void t2(void *)
{
    int rsense = 1;
    int rtdiff0 = -1;
    int rtdiff1 = -1;
    int rparentsense2 = -1;
    for(int i=0;i<LOOP;i++)
    {
        atomic_store_explicit(__FILE__, __LINE__, &tdiff2, 1, memory_order_release);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        atomic_store_explicit(__FILE__, __LINE__, &childnotready0_1, 0, memory_order_release);
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rparentsense2 = atomic_load_explicit(__FILE__, __LINE__, &parentsense2, memory_order_acquire);
        if(!(rparentsense2==rsense)) return;
        rsense = 1 - rsense;
        atomic_fetch_add_explicit(__FILE__, __LINE__, &__fence_var, 0, memory_order_acq_rel);
        rtdiff0 = atomic_load_explicit(__FILE__, __LINE__, &tdiff0, memory_order_acquire);
        rtdiff1 = atomic_load_explicit(__FILE__, __LINE__, &tdiff1, memory_order_acquire);
        MODEL_ASSERT(rtdiff0!=0 && rtdiff1 !=0);
    }
}


int *propertyChecking(void* arg)
{
    return 0;
}

int user_main(int argc, char **argv)
{
    thrd_t _t1, _t2, _t3;
    atomic_init(&childnotready0_0, 1);
    atomic_init(&childnotready0_1, 1);
    atomic_init(&parentsense1,0); 

    atomic_init(&__fence_var,  0);
    atomic_init(&parentsense2, 0);
    atomic_init(&tdiff0, 0);
    atomic_init(&tdiff1, 0);
    atomic_init(&tdiff2, 0);
    thrd_create(&_t1, t0, 0);
    thrd_create(&_t2, t1, 0);
    thrd_create(&_t3, t2, 0);
    return 0;
}



