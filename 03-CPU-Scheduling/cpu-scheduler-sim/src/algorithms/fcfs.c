#include "fcfs.h"
#include "../../include/common.h"
#include "../utils/metrics.h"

void sort_by_arrival_time(Process processes[], int num_processes) {
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

void schedule_fcfs(Process processes[], int num_processes) {
    printf("\n=== FCFS (First-Come First-Served) Scheduling ===\n");
    
    // Sort processes by arrival time
    sort_by_arrival_time(processes, num_processes);
    
    int current_time = 0;
    
    for (int i = 0; i < num_processes; i++) {
        // If CPU is idle, jump to next process arrival
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        // Set response time (first time process gets CPU)
        processes[i].response_time = current_time - processes[i].arrival_time;
        processes[i].started = 1;
        
        // Execute process completely
        current_time += processes[i].burst_time;
        processes[i].completion_time = current_time;
        
        printf("Process P%d: Start=%d, Finish=%d\n", 
               processes[i].id, current_time - processes[i].burst_time, current_time);
    }
    
    // Calculate and print metrics
    SchedulingMetrics metrics;
    calculate_metrics(processes, num_processes, &metrics);
    
    printf("\nProcess Details:\n");
    for (int i = 0; i < num_processes; i++) {
        print_process(&processes[i]);
    }
    
    print_metrics(&metrics);
    print_gantt_chart(processes, num_processes);
}