#include <stdio.h>
#include "thread.h"

//compile with gcc -o test test.c FIFO.c SJF.c quantum.c calculations.c

//functions
void fifo(struct thread* jobs, int numjobs);
void sjf_scheduler(struct thread* jobs, int num_jobs);
void hybrid_quantum(struct thread *jobs, int numjobs, int quantum);
void print_comparison_table(struct thread* fifo, struct thread* sjf, struct thread* hybrid, int n);


int main() {

    int QUANTUM = 2;

    // Test Case 1: Basic
    printf("TEST CASE 1: Basic 3 Jobs\n");
    struct thread jobs1[] = {
        { .thread_ID = 1, .arrival = 0, .burst = 10, .turnaround = 0, .response = 0, .wait = 0 },
        { .thread_ID = 2, .arrival = 2, .burst = 4,  .turnaround = 0, .response = 0, .wait = 0 },
        { .thread_ID = 3, .arrival = 4, .burst = 6,  .turnaround = 0, .response = 0, .wait = 0 }
    };

    int num_jobs1 = sizeof(jobs1) / sizeof(jobs1[0]);
    struct thread jobs1_fifo[3];
    struct thread jobs1_sjf[3];
    struct thread jobs1_quantum[3];

    for (int i = 0; i < num_jobs1; i++) {
        jobs1_fifo[i] = jobs1[i];
        jobs1_sjf[i] = jobs1[i];
        jobs1_quantum[i] = jobs1[i];
    }

    printf("    SJF Scheduler:\n");
    sjf_scheduler(jobs1_sjf, num_jobs1);

    printf("    FIFO Scheduler:\n");
    fifo(jobs1_fifo, num_jobs1);

    printf("    Hybrid Quantum (Q=%d):\n", QUANTUM);
    hybrid_quantum(jobs1_quantum, num_jobs1, QUANTUM);

    print_comparison_table(jobs1_fifo, jobs1_sjf, jobs1_quantum, num_jobs1);



    // Test Case 2: Same arrival time
    printf("\nTEST CASE 2: Same Arrival Time\n");
    struct thread jobs2[] = {
        { .thread_ID = 1, .arrival = 0, .burst = 8, .turnaround = 0, .response = 0, .wait = 0 },
        { .thread_ID = 2, .arrival = 0, .burst = 4, .turnaround = 0, .response = 0, .wait = 0 },
        { .thread_ID = 3, .arrival = 0, .burst = 9, .turnaround = 0, .response = 0, .wait = 0 },
        { .thread_ID = 4, .arrival = 0, .burst = 5, .turnaround = 0, .response = 0, .wait = 0 }
    };

    int num_jobs2 = sizeof(jobs2) / sizeof(jobs2[0]);
    struct thread jobs2_fifo[4];
    struct thread jobs2_sjf[4];
    struct thread jobs2_quantum[4];

    for (int i = 0; i < num_jobs2; i++) {
        jobs2_fifo[i] = jobs2[i];
        jobs2_sjf[i] = jobs2[i];
        jobs2_quantum[i] = jobs2[i];
    }

    printf("    SJF Scheduler:\n");
    sjf_scheduler(jobs2_sjf, num_jobs2);

    printf("    FIFO Scheduler:\n");
    fifo(jobs2_fifo, num_jobs2);

    printf("    Hybrid Quantum (Q=%d):\n", QUANTUM);
    hybrid_quantum(jobs2_quantum, num_jobs2, QUANTUM);

    print_comparison_table(jobs2_fifo, jobs2_sjf, jobs2_quantum, num_jobs2);

    // Test Case 3: Short job arrives later
    printf("\nTEST CASE 3: Short Job Arrives Later\n");
    struct thread jobs3[] = {
        { .thread_ID = 1, .arrival = 0, .burst = 10, .turnaround = 0, .response = 0, .wait = 0 },
        { .thread_ID = 2, .arrival = 1, .burst = 1,  .turnaround = 0, .response = 0, .wait = 0 },
        { .thread_ID = 3, .arrival = 2, .burst = 8,  .turnaround = 0, .response = 0, .wait = 0 }
    };

    int num_jobs3 = sizeof(jobs3) / sizeof(jobs3[0]);
    struct thread jobs3_fifo[3];
    struct thread jobs3_sjf[3];
    struct thread jobs3_quantum[3];

    for (int i = 0; i < num_jobs3; i++) {
        jobs3_fifo[i] = jobs3[i];
        jobs3_sjf[i] = jobs3[i];
        jobs3_quantum[i] = jobs3[i];
    }

    printf("    SJF Scheduler:\n");
    sjf_scheduler(jobs3_sjf, num_jobs3);

    printf("    FIFO Scheduler:\n");
    fifo(jobs3_fifo, num_jobs3);

    printf("    Hybrid Quantum (Q=%d):\n", QUANTUM);
    hybrid_quantum(jobs3_quantum, num_jobs3, QUANTUM);

    print_comparison_table(jobs3_fifo, jobs3_sjf, jobs3_quantum, num_jobs3);

    // Test Case 4: Single job
    printf("\nTEST CASE 4: Single Job\n");
    struct thread jobs4[] = {
        { .thread_ID = 1, .arrival = 0, .burst = 5, .turnaround = 0, .response = 0, .wait = 0 }
    };

    int num_jobs4 = sizeof(jobs4) / sizeof(jobs4[0]);
    struct thread jobs4_fifo[1];
    struct thread jobs4_sjf[1];
    struct thread jobs4_quantum[1];

    for (int i = 0; i < num_jobs4; i++) {
        jobs4_fifo[i] = jobs4[i];
        jobs4_sjf[i] = jobs4[i];
        jobs4_quantum[i] = jobs4[i];
    }

    printf("    SJF Scheduler:\n");
    sjf_scheduler(jobs4_sjf, num_jobs4);

    printf("    FIFO Scheduler:\n");
    fifo(jobs4_fifo, num_jobs4);

    printf("    Hybrid Quantum (Q=%d):\n", QUANTUM);
    hybrid_quantum(jobs4_quantum, num_jobs4, QUANTUM);

    print_comparison_table(jobs4_fifo, jobs4_sjf, jobs4_quantum, num_jobs4);

    // Test Case 5: Quantum Implementation
    printf("\nTEST CASE 5: Quantum Implementation\n");
    struct thread jobs5[] = {
        { .thread_ID = 1, .arrival = 0, .burst = 8, .turnaround = 0, .response = 0, .wait = 0, .completion_time = 0, .first_run = -1 },
        { .thread_ID = 2, .arrival = 1, .burst = 6, .turnaround = 0, .response = 0, .wait = 0, .completion_time = 0, .first_run = -1 },
        { .thread_ID = 3, .arrival = 2, .burst = 4, .turnaround = 0, .response = 0, .wait = 0, .completion_time = 0, .first_run = -1 }
    };

    int num_jobs5 = sizeof(jobs5) / sizeof(jobs5[0]);
    struct thread jobs5_fifo[3];
    struct thread jobs5_sjf[3];
    struct thread jobs5_quantum[3];

    for (int i = 0; i < num_jobs5; i++) {
        jobs5_fifo[i] = jobs5[i];
        jobs5_sjf[i] = jobs5[i];
        jobs5_quantum[i] = jobs5[i];
    }

    printf("    SJF Scheduler:\n");
    sjf_scheduler(jobs5_sjf, num_jobs5);

    printf("    FIFO Scheduler:\n");
    fifo(jobs5_fifo, num_jobs5);

    printf("    Hybrid Scheduler [FIFO → SJF at quantum=%d]:\n", QUANTUM);
    hybrid_quantum(jobs5_quantum, num_jobs5, QUANTUM);

    print_comparison_table(jobs5_fifo, jobs5_sjf, jobs5_quantum, num_jobs5);
    return 0;
}

