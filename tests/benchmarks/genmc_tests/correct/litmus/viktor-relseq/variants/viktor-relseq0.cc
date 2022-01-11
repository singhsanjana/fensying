#include <threads.h>#include <stdatomic.h>

#include "../viktor-relseq.c"

int main()
{
        thrd_t R0, R1, R2, R3;

        thrd_create(&R0, (thrd_start_t)& threadR, NULL);
        thrd_create(&R1, (thrd_start_t)& threadRa, NULL);
        thrd_create(&R2, (thrd_start_t)& threadRr, NULL);
        thrd_create(&R3, (thrd_start_t)& threadRs, NULL);

        return 0;
}
