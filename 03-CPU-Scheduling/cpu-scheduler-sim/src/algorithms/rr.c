#include "rr.h"
#include "../../include/common.h"
#include "../utils/metrics.h"

void schedule_round_robin(Process processes[], int num_processes, int time_quantum) {
    printf("\n=== Round Robin Scheduling (Quantum = %d) ===\n", time_quantum);
    
    Queue ready_queue;
    init_queue(&ready_queue);
    
    int current_time = 0;
    int completed = 0;
    int process_index = 0;
    
    // Add first process that arrives at time 0
    while (process_index < num_processes && processes[process_index].arrival_time <= current_time) {
        enqueue(&ready_queue, &processes[process_index]);
        process_index++;
    }
    
    while (completed < num_processes) {
        if (is_empty(&ready_queue)) {
            // No process in queue, advance time
            current_time++;
            // Check for new arrivals
            while (process_index < num_processes && processes[process_index].arrival_time <= current_time) {
                enqueue(&ready_queue, &processes[process_index]);
                process_index++;
            }
            continue;
        }
        
        Process* current_process = dequeue(&ready_queue);
        
        // Set response time if first time
        if (!current_process->started) {
            current_process->response_time = current_time - current_process->arrival_time;
            current_process->started = 1;
        }
        
        // Execute for time quantum or remaining time, whichever is smaller
        int execute_time = (current_process->remaining_time < time_quantum) ? 
                          current_process->remaining_time : time_quantum;
        
        printf("Process P%d executes from time %d to %d\n", 
               current_process->id, current_time, current_time + execute_time);
        
        current_process->remaining_time -= execute_time;
        current_time += execute_time;
        
        // Check for new arrivals during execution
        while (process_index < num_processes && processes[process_index].arrival_time <= current_time) {
            enqueue(&ready_queue, &processes[process_index]);
            process_index++;
        }
        
        // Check if process completed
        if (current_process->remaining_time == 0) {
            current_process->completion_time = current_time;
            completed++;
            printf("Process P%d completed\n", current_process->id);
        } else {
            // Add back to queue if not completed
            enqueue(&ready_queue, current_process);
        }
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