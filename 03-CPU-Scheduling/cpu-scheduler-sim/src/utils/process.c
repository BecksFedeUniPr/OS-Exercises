#include "process.h"
#include "../../include/common.h"

void init_process(Process* p, int id, int arrival, int burst, int priority) {
    p->id = id;
    p->arrival_time = arrival;
    p->burst_time = burst;
    p->remaining_time = burst;
    p->priority = priority;
    p->completion_time = 0;
    p->turnaround_time = 0;
    p->waiting_time = 0;
    p->response_time = -1;
    p->started = 0;
}

void reset_process_metrics(Process* p) {
    p->remaining_time = p->burst_time;
    p->completion_time = 0;
    p->turnaround_time = 0;
    p->waiting_time = 0;
    p->response_time = -1;
    p->started = 0;
}

void print_process(Process* p) {
    printf("P%d: AT=%d, BT=%d, CT=%d, TAT=%d, WT=%d, RT=%d\n",
           p->id, p->arrival_time, p->burst_time, p->completion_time,
           p->turnaround_time, p->waiting_time, p->response_time);
}

void copy_processes(Process* dest, Process* src, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
        reset_process_metrics(&dest[i]);
    }
}