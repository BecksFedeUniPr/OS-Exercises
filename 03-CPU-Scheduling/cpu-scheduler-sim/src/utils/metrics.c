#include "metrics.h"
#include <stdio.h>

void calculate_metrics(Process processes[], int num_processes, SchedulingMetrics* metrics) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int total_response_time = 0;
    int max_completion_time = 0;
    
    for (int i = 0; i < num_processes; i++) {
        // Calculate turnaround time: completion_time - arrival_time
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        
        // Calculate waiting time: turnaround_time - burst_time
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_response_time += processes[i].response_time;
        
        if (processes[i].completion_time > max_completion_time) {
            max_completion_time = processes[i].completion_time;
        }
    }
    
    metrics->avg_waiting_time = (double)total_waiting_time / num_processes;
    metrics->avg_turnaround_time = (double)total_turnaround_time / num_processes;
    metrics->avg_response_time = (double)total_response_time / num_processes;
    metrics->throughput = (double)num_processes / max_completion_time;
}

void print_metrics(SchedulingMetrics* metrics) {
    printf("\n=== Scheduling Metrics ===\n");
    printf("Average Waiting Time: %.2f\n", metrics->avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", metrics->avg_turnaround_time);
    printf("Average Response Time: %.2f\n", metrics->avg_response_time);
    printf("Throughput: %.2f processes/unit time\n", metrics->throughput);
}

void print_gantt_chart(Process processes[], int num_processes) {
    printf("\n=== Gantt Chart ===\n");
    printf("Timeline: ");
    for (int i = 0; i < num_processes; i++) {
        int start_time = processes[i].completion_time - processes[i].burst_time;
        printf("P%d(%d-%d) ", processes[i].id, start_time, processes[i].completion_time);
    }
    printf("\n");
}