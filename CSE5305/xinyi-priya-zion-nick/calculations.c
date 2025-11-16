#include <stdio.h>
#include "thread.h"
void print_comparison_table( struct thread* fifo, struct thread* sjf, struct thread* hybrid, int n)
{
    double fifo_wait = 0, fifo_turn = 0, fifo_resp = 0;
    double sjf_wait = 0, sjf_turn = 0, sjf_resp = 0;
    double hyb_wait = 0, hyb_turn = 0, hyb_resp = 0;

    for (int i = 0; i < n; i++) {
        fifo_wait += fifo[i].wait;
        fifo_turn += fifo[i].turnaround;
        fifo_resp += fifo[i].response;

        sjf_wait += sjf[i].wait;
        sjf_turn += sjf[i].turnaround;
        sjf_resp += sjf[i].response;

        hyb_wait += hybrid[i].wait;
        hyb_turn += hybrid[i].turnaround;
        hyb_resp += hybrid[i].response;
    }

    printf("Scheduler   | Avg Wait | Avg Turnaround | Avg Response\n");
    printf("===========================================================\n");
    printf("FIFO        | %8.2f | %14.2f | %12.2f\n", fifo_wait/n, fifo_turn/n, fifo_resp/n);
    printf("SJF         | %8.2f | %14.2f | %12.2f\n", sjf_wait/n, sjf_turn/n, sjf_resp/n);
    printf("Hybrid      | %8.2f | %14.2f | %12.2f\n", hyb_wait/n, hyb_turn/n, hyb_resp/n);
    printf("===========================================================\n\n");
}
