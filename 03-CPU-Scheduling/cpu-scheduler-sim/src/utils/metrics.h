#ifndef METRICS_H
#define METRICS_H

#include "process.h"

// Scheduling metrics structure
typedef struct {
    double avg_waiting_time;
    double avg_turnaround_time;
    double avg_response_time;
    double throughput;
} SchedulingMetrics;

// Function prototypes
void calculate_metrics(Process processes[], int num_processes, SchedulingMetrics* metrics);
void print_metrics(SchedulingMetrics* metrics);
void print_gantt_chart(Process processes[], int num_processes);

#endif