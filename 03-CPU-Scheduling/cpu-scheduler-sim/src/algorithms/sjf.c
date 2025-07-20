#include "sjf.h"
#include "../../include/common.h"
#include "../utils/metrics.h"

int find_shortest_job(Process processes[], int num_processes, int current_time) {
    int shortest_idx = -1;
    int min_burst_time = 99999;
    
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time <= current_time && 
            processes[i].completion_time == 0 && 
            processes[i].burst_time < min_burst_time) {
            min_burst_time = processes[i].burst_time;
            shortest_idx = i;
        }
    }
    
    return shortest_idx;
}

void schedule_sjf(Process processes[], int num_processes) {
    printf("\n=== SJF (Shortest Job First) Scheduling ===\n");
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < num_processes) {
        int next_process = find_shortest_job(processes, num_processes, current_time);
        
        if (next_process == -1) {
            // No process available, advance time
            current_time++;
            continue;
        }
        
        // Set response time
        processes[next_process].response_time = current_time - processes[next_process].arrival_time;
        
        // Execute process completely
        current_time += processes[next_process].burst_time;
        processes[next_process].completion_time = current_time;
        completed++;
        
        printf("Process P%d: Start=%d, Finish=%d\n", 
               processes[next_process].id, 
               current_time - processes[next_process].burst_time, 
               current_time);
    }
    
    // Calculate and print metrics
    SchedulingMetrics metrics;
    calculate_metrics(processes, num_processes, &metrics);
    
    printf("\nProcess Details:\n");
    for (int i = 0; i < num_processes; i++) {
        print_process(&processes[i]);
    }
    
    print_metrics(&metrics);
}