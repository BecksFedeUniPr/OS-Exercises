#include "../src/utils/process.h"
#include "../src/algorithms/fcfs.h"
#include "../src/algorithms/sjf.h"
#include "../src/algorithms/srtf.h"
#include "../src/algorithms/rr.h"
#include "../src/algorithms/priority.h"
#include "../include/common.h"

void run_fcfs_example() {
    printf("\n=== FCFS Example ===\n");
    Process processes[] = {
        {1, 0, 10, 3, 0}, // remaining_time inizializzato a 0 (non usato in FCFS)
        {2, 1, 4, 1, 0},
        {3, 2, 6, 2, 0}
    };
    int num_processes = 3;
    
    // Initialize processes
    for (int i = 0; i < num_processes; i++) {
        init_process(&processes[i], processes[i].id, processes[i].arrival_time,
                    processes[i].burst_time, processes[i].priority);
    }
    
    schedule_fcfs(processes, num_processes);
}

void run_sjf_example() {
    printf("\n=== SJF Example ===\n");
    Process processes[] = {
        {1, 0, 7, 0, 7}, // remaining_time uguale a burst_time (usato per selezione)
        {2, 2, 4, 0, 4},
        {3, 4, 1, 0, 1},
        {4, 5, 4, 0, 4}
    };
    int num_processes = 4;
    
    // Initialize processes
    for (int i = 0; i < num_processes; i++) {
        init_process(&processes[i], processes[i].id, processes[i].arrival_time,
                    processes[i].burst_time, processes[i].priority);
    }
    
    schedule_sjf(processes, num_processes);
}

void run_rr_example() {
    printf("\n=== Round Robin Example ===\n");
    Process processes[] = {
        {1, 0, 10, 0, 10}, // remaining_time uguale a burst_time (decrementato durante l'esecuzione)
        {2, 1, 4, 0, 4},
        {3, 2, 6, 0, 6}
    };
    int num_processes = 3;
    
    // Initialize processes
    for (int i = 0; i < num_processes; i++) {
        init_process(&processes[i], processes[i].id, processes[i].arrival_time,
                    processes[i].burst_time, processes[i].priority);
    }
    
    schedule_round_robin(processes, num_processes, 3);
}

void run_priority_example() {
    printf("\n=== Priority Scheduling Example ===\n");
    Process processes[] = {
        {1, 0, 10, 3, 10}, // remaining_time uguale a burst_time (decrementato se preemptive)
        {2, 1, 4, 1, 4},
        {3, 2, 6, 2, 6},
        {4, 3, 8, 4, 8}
    };
    int num_processes = 4;
    
    // Initialize processes
    for (int i = 0; i < num_processes; i++) {
        init_process(&processes[i], processes[i].id, processes[i].arrival_time,
                    processes[i].burst_time, processes[i].priority);
    }
    
    schedule_priority_non_preemptive(processes, num_processes);
}

int main() {
    printf("=== CPU Scheduling Algorithms Demo ===\n");
    
    run_fcfs_example();
    run_sjf_example();
    run_rr_example();
    run_priority_example();
    
    return 0;
}