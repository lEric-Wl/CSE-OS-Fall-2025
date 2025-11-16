#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct 
{
    int pid; // process ID
    int arrival; // arrival time 
    int burst; // CPU burst time
    int remaining; // remain burst time 
    int turnaround; // turnaround time
    int waiting; // waiting time
    int response; // response time
    int started; // first start
    int completion; // finish time
} Process;

// algorithms
void fcfs(Process p[], int n);
void sjf(Process p[], int n);
void srtf(Process p[], int n);
void rr(Process p[], int n, int quantum);

// print functions
void printTable(Process p[], int n);
void printAverages(Process p[], int n);
void printGanttChart(int timeline[], int timeline_pids[], int length);

#endif
