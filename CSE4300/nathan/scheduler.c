#include <stdio.h>
#include <limits.h>
#include "scheduler.h"


// print table function
void printTable(Process p[], int n) 
{
    printf("\nPID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\tResponse\n");
    for (int i = 0; i < n; i++) 
    {
        // time metrics for processes
        printf("P%d\t%d\t%d\t%d\t\t%d\t\t%d\t%d\n",
            p[i].pid, p[i].arrival, p[i].burst,
            p[i].completion, p[i].turnaround, p[i].waiting, p[i].response);
    }
}

// calculate and print TT WT and RT averages
void printAverages(Process p[], int n) 
{
    float totalTAT = 0, totalWT = 0, totalRT = 0;
    for (int i = 0; i < n; i++) 
    {
        totalTAT += p[i].turnaround;
        totalWT += p[i].waiting;
        totalRT += p[i].response;
    }
    printf("\nAverage Turnaround Time: %.2f", totalTAT / n);
    printf("\nAverage Waiting Time: %.2f", totalWT / n);
    printf("\nAverage Response Time: %.2f\n", totalRT / n);
}

// print the gantt chart
void printGanttChart(int timeline[], int timeline_pids[], int length) 
{
    printf("\nGantt Chart:\n");
    printf(" ");
    for (int i = 0; i < length; i++) 
    {
        printf("----"); // top border
    }
    printf("-\n|");
    for (int i = 0; i < length; i++) 
    {
        printf("P%d |", timeline_pids[i]); // process
    }
    printf("\n ");
    for (int i = 0; i < length; i++) 
    {
        printf("----"); // bottom border
    }
    printf("-\n0");
    for (int i = 0; i < length; i++) 
    {
        printf("%4d", timeline[i]); // timeline
    }
    printf("\n");
}

// first come first serve
void fcfs(Process p[], int n) 
{
    int time = 0;
    int timeline[n], timeline_pids[n], len = 0;
    for (int i = 0; i < n; i++) 
    {
        // if idle, move to next arrival time
        if (time < p[i].arrival)
            time = p[i].arrival;
        p[i].response = time - p[i].arrival; // RT = first exec - arrival
        time += p[i].burst; // run to completion
        p[i].completion = time; 
        p[i].turnaround = p[i].completion - p[i].arrival; // TT = completion - arrival
        p[i].waiting = p[i].turnaround - p[i].burst; // WT = TT - burst
        timeline[len] = time; // len update for Gantt chart
        timeline_pids[len] = p[i].pid;
        len++;
    }

    printf("\n--- FCFS ---\n");
    printTable(p, n);
    printGanttChart(timeline, timeline_pids, len);
    printAverages(p, n);
}

// shortest job first
void sjf(Process p[], int n) 
{
    int completed = 0, time = 0;
    int isDone[n]; 
    int timeline[100], timeline_pids[100], len = 0;
    for (int i = 0; i < n; i++) isDone[i] = 0;

    while (completed < n) 
    {
        int idx = -1, minBurst = INT_MAX;
        for (int i = 0; i < n; i++)  // find shortest available job
        {
            if (!isDone[i] && p[i].arrival <= time && p[i].burst < minBurst) 
            {
                minBurst = p[i].burst;
                idx = i;
            }
        }

        // increment time if nothing ready
        if (idx == -1) {
            time++;
            continue;
        }

        // assign values, same as in FCFS
        p[idx].response = time - p[idx].arrival;
        time += p[idx].burst;
        p[idx].completion = time;
        p[idx].turnaround = p[idx].completion - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        isDone[idx] = 1;
        completed++;
        timeline[len] = time;
        timeline_pids[len++] = p[idx].pid;
    }

    printf("\n--- SJF ---\n");
    printTable(p, n);
    printGanttChart(timeline, timeline_pids, len);
    printAverages(p, n);
}

// shortest remaining time first
void srtf(Process p[], int n) 
{
    int completed = 0, time = 0;
    int shortest = -1, minRemaining = INT_MAX;
    int totalBurst = 0;
    for (int i = 0; i < n; i++) totalBurst += p[i].burst;

    int timeline[200], timeline_pids[200], len = 0;
    int lastPid = -1;

    while (completed < n) 
    {
        shortest = -1;
        minRemaining = INT_MAX;
        for (int j = 0; j < n; j++) // find process with shortest completion time remaining
        {
            if (p[j].arrival <= time && p[j].remaining > 0 && p[j].remaining < minRemaining) 
            {
                minRemaining = p[j].remaining;
                shortest = j;
            }
        }

        // increment time if nothing ready
        if (shortest == -1) 
        {
            time++;
            continue;
        }

        // record first started if first process
        if (!p[shortest].started) 
        {
            p[shortest].response = time - p[shortest].arrival;
            p[shortest].started = 1;
        }

        // one unit of time forward
        p[shortest].remaining--;
        time++;

        // check if process changes or not
        if (p[shortest].pid != lastPid) 
        {
            timeline_pids[len] = p[shortest].pid;
            timeline[len] = time;
            len++;
            lastPid = p[shortest].pid;
        }

        // assign values
        if (p[shortest].remaining == 0) 
        {
            p[shortest].completion = time;
            p[shortest].turnaround = p[shortest].completion - p[shortest].arrival;
            p[shortest].waiting = p[shortest].turnaround - p[shortest].burst;
            completed++;
        }
    }

    printf("\n--- SRTF ---\n");
    printTable(p, n);
    printGanttChart(timeline, timeline_pids, len);
    printAverages(p, n);
}

// round robin
void rr(Process p[], int n, int quantum) 
{
    int time = 0, done = 0;
    int timeline[200], timeline_pids[200], len = 0;
    for (int i = 0; i < n; i++) p[i].started = 0; // init started flags

    while (1) 
    {
        done = 1;
        for (int i = 0; i < n; i++) 
        {
            if (p[i].remaining > 0 && p[i].arrival <= time) 
            {
                done = 0; // not done
                if (!p[i].started) // first exec RT
                {
                    p[i].response = time - p[i].arrival; 
                    p[i].started = 1;
                }
                // execute a quantum or until finished process
                int execTime = (p[i].remaining > quantum) ? quantum : p[i].remaining;
                time += execTime;
                p[i].remaining -= execTime;
                timeline_pids[len] = p[i].pid;
                timeline[len] = time;
                len++;

                if (p[i].remaining == 0)  // assign values
                {
                    p[i].completion = time;
                    p[i].turnaround = p[i].completion - p[i].arrival;
                    p[i].waiting = p[i].turnaround - p[i].burst;
                }
            }
        }
        if (done) break;
    }

    printf("\n--- Round Robin (q=%d) ---\n", quantum);
    printTable(p, n);
    printGanttChart(timeline, timeline_pids, len);
    printAverages(p, n);
}
