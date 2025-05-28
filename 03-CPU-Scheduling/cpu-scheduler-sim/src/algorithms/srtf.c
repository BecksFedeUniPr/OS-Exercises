#include "srtf.h"
#include "../../include/common.h"
#include "../utils/metrics.h"

int find_shortest_remaining_time(Process processes[], int num_processes, int current_time) {
    int shortest_idx = -1;
    int min_remaining_time = 99999;
    
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time <= current_time && 
            processes[i].remaining_time > 0 && 
            processes[i].remaining_time < min_remaining_time) {
            min_remaining_time = processes[i].remaining_time;
            shortest_idx = i;
        }
    }
    
    return shortest_idx;
}

void schedule_srtf(Process processes[], int num_processes) {
    printf("\n=== SRTF (Shortest Remaining Time First) Scheduling ===\n");
    
    int current_time = 0;
    int completed = 0;
    int last_process = -1;
    
    while (completed < num_processes) {
        int next_process = find_shortest_remaining_time(processes, num_processes, current_time);
        
        if (next_process == -1) {
            current_time++;
            continue;
        }
        
        // Set response time if process starts for first time
        if (!processes[next_process].started) {
            processes[next_process].response_time = current_time - processes[next_process].arrival_time;
            processes[next_process].started = 1;
        }
        
        // Print context switch if different process
        if (last_process != next_process && last_process != -1) {
            printf("Context switch at time %d: P%d -> P%d\n", current_time, last_process, processes[next_process].id);
        }
        
        // Execute for 1 time unit
        processes[next_process].remaining_time--;
        current_time++;
        
        // Check if process completed
        if (processes[next_process].remaining_time == 0) {
            processes[next_process].completion_time = current_time;
            completed++;
            printf("Process P%d completed at time %d\n", processes[next_process].id, current_time);
        }
        
        last_process = processes[next_process].id;
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