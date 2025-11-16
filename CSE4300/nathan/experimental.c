#include <stdio.h>
#include "scheduler.h"

// FCFS advantage; shortest jobs first
int data1[][2] = { {0, 1}, {1, 2}, {2, 3} };
#define N1 3 // number of processes
#define Q1 2 // quantum of RR

// FCFS disadvantage; longest jobs first
int data2[][2] = { {0, 3}, {1, 2}, {2, 1} };
#define N2 3
#define Q2 2

// SJF advantage; short jobs are available before longer ones
int data3[][2] = { {0, 1}, {0, 10}, {5, 5} };
#define N3 3
#define Q3 3

// SJF disadvantage; short jobs starve longer jobs
int data4[][2] = { {0, 10}, {0, 1}, {1, 2}, {1, 1}, {1, 2}, {1, 1} };
#define N4 6
#define Q4 2

// SRTF advantage; late short job can be adjusted to 
int data5[][2] = { {0, 1}, {0, 10}, {5, 5} };
#define N5 3
#define Q5 3

// SRTF disadvantage; frequent context switches extend early job
int data6[][2] = { {0, 10}, {0, 1}, {1, 2}, {1, 1}, {1, 2}, {1, 1} };
#define N6 6
#define Q6 2

// RR advantage; splits equal jobs equally
int data7[][2] = { {0, 5}, {0, 5}, {0, 5}, {0, 5} };
#define N7 4
#define Q7 2

// RR disadvantage; extends longer-medium jobs if many smaller jobs are alongside and the quantum is too small
int data8[][2] = { {0, 10}, {0, 1}, {1, 2}, {1, 1}, {1, 2}, {1, 1} };
#define N8 6
#define Q8 2

int *datasets[]      = { (int*)data1, (int*)data2, (int*)data3, (int*)data4, (int*)data5, (int*)data6, (int*)data7, (int*)data8 };
int dataset_sizes[]  = { N1, N2, N3, N4, N5, N6, N7, N8 };
int dataset_quanta[] = { Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8 };

void runDataset(int id, void (*algorithm)(Process*, int), const char *name) {
    int n = dataset_sizes[id];
    int *data = datasets[id];
    Process p[n];
    for (int i = 0; i < n; i++) 
    {
        p[i].pid = i + 1;
        p[i].arrival = data[i * 2];
        p[i].burst = data[i * 2 + 1];
        p[i].remaining = p[i].burst;
        p[i].started = 0;
    }
    printf("\n===============================\n");
    printf("%s - Dataset %d\n", name, id + 1);
    printf("===============================\n");
    algorithm(p, n);
}

int main() 
{
    runDataset(0, fcfs, "FCFS (Advantage)");
    runDataset(1, fcfs, "FCFS (Disadvantage)");
    runDataset(2, sjf, "SJF (Advantage)");
    runDataset(3, sjf, "SJF (Disadvantage)");
    runDataset(4, srtf, "SRTF (Advantage)");
    runDataset(5, srtf, "SRTF (Disadvantage)");


    // running the RR dataset separate because of the extra quantum variable
    printf("\n===============================\n");
    printf("RR (Advantage) - Dataset 7\n");
    printf("===============================\n");
    {
        int id = 6;
        int n = dataset_sizes[id];
        int *data = datasets[id];
        Process p[n];
        for (int i = 0; i < n; i++) 
        {
            p[i].pid = i + 1;
            p[i].arrival = data[i * 2];
            p[i].burst = data[i * 2 + 1];
            p[i].remaining = p[i].burst;
            p[i].started = 0;
        }
        rr(p, n, dataset_quanta[id]);
    }

    printf("\n===============================\n");
    printf("RR (Disadvantage) - Dataset 8\n");
    printf("===============================\n");
    {
        int id = 7;
        int n = dataset_sizes[id];
        int *data = datasets[id];
        Process p[n];
        for (int i = 0; i < n; i++) 
        {
            p[i].pid = i + 1;
            p[i].arrival = data[i * 2];
            p[i].burst = data[i * 2 + 1];
            p[i].remaining = p[i].burst;
            p[i].started = 0;
        }
        rr(p, n, dataset_quanta[id]);
    }

    return 0;
}
