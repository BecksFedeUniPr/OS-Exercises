#include "priority.h"
#include "../../include/common.h"
#include "../utils/metrics.h"

int find_highest_priority(Process processes[], int num_processes, int current_time, int preemptive) {
    int highest_priority_idx = -1;
    int highest_priority = 99999; // Lower number = higher priority
    
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time <= current_time) {
            if (preemptive) {
                if (processes[i].remaining_time > 0 && processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    highest_priority_idx = i;
                }
            } else {
                if (processes[i].completion_time == 0 && processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    highest_priority_idx = i;
                }
            }
        }
    }
    
    return highest_priority_idx;
}

void schedule_priority_non_preemptive(Process processes[], int num_processes) {
    printf("\n=== Priority Scheduling (Non-Preemptive) ===\n");
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < num_processes) {
        int next_process = find_highest_priority(processes, num_processes, current_time, 0);
        
        if (next_process == -1) {
            current_time++;
            continue;
        }
        
        // Set response time
        processes[next_process].response_time = current_time - processes[next_process].arrival_time;
        
        // Execute process completely
        printf("Process P%d (Priority %d) executes from %d to %d\n", 
               processes[next_process].id, processes[next_process].priority,
               current_time, current_time + processes[next_process].burst_time);
        
        current_time += processes[next_process].burst_time;
        processes[next_process].completion_time = current_time;
        completed++;
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

void schedule_priority_preemptive(Process processes[], int num_processes) {
    printf("\n=== Priority Scheduling (Preemptive) ===\n");
    
    int current_time = 0;
    int completed = 0;
    int last_process = -1;
    
    while (completed < num_processes) {
        int next_process = find_highest_priority(processes, num_processes, current_time, 1);
        
        if (next_process == -1) {
            current_time++;
            continue;
        }
        
        // Set response time if first time
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