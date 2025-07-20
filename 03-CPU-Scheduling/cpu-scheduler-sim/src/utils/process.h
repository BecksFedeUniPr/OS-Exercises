#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int started;
} Process;

void init_process(Process* p, int id, int arrival, int burst, int priority);
void reset_process_metrics(Process* p);
void print_process(Process* p);
void copy_processes(Process* dest, Process* src, int n);

#endif