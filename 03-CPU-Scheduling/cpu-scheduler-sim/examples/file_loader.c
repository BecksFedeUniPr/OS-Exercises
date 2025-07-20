#include <stdio.h>
#include <stdlib.h>
#include "../src/utils/process.h"
#include "../src/algorithms/fcfs.h"
#include "../src/algorithms/sjf.h"
#include "../src/algorithms/srtf.h"
#include "../src/algorithms/rr.h"
#include "../src/algorithms/priority.h"
#include "../include/common.h"

// Utility to load processes from file
int load_processes_from_file(const char* filename, Process* processes, int max_size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }
    
    int num_processes;
    fscanf(file, "%d", &num_processes);
    
    if (num_processes > max_size) {
        printf("Warning: File contains %d processes, but only %d will be loaded\n", 
               num_processes, max_size);
        num_processes = max_size;
    }
    
    for (int i = 0; i < num_processes; i++) {
        fscanf(file, "%d %d %d %d", &processes[i].id, &processes[i].arrival_time,
               &processes[i].burst_time, &processes[i].priority);
        
        init_process(&processes[i], processes[i].id, processes[i].arrival_time,
                    processes[i].burst_time, processes[i].priority);
    }
    
    fclose(file);
    return num_processes;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    Process processes[MAX_PROCESSES];
    int num_processes = load_processes_from_file(argv[1], processes, MAX_PROCESSES);
    
    if (num_processes < 0) {
        return 1;
    }
    
    printf("Loaded %d processes from file %s\n\n", num_processes, argv[1]);
    printf("Process | Arrival | Burst | Priority\n");
    printf("--------|---------|-------|--------\n");
    
    for (int i = 0; i < num_processes; i++) {
        printf("P%-6d | %-7d | %-5d | %-7d\n", processes[i].id, 
               processes[i].arrival_time, processes[i].burst_time, processes[i].priority);
    }

    // Selezione dell'algoritmo di scheduling
    printf("\nSelect Scheduling Algorithm:\n");
    printf("1. FCFS (First-Come First-Served)\n");
    printf("2. SJF (Shortest Job First)\n");
    printf("3. SRTF (Shortest Remaining Time First)\n");
    printf("4. RR (Round Robin)\n");
    printf("5. Priority Scheduling\n");
    printf("Enter your choice: ");
    
    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\nExecuting FCFS...\n");
            schedule_fcfs(processes, num_processes);
            break;
        case 2:
            printf("\nExecuting SJF...\n");
            schedule_sjf(processes, num_processes);
            break;
        case 3:
            printf("\nExecuting SRTF...\n");
            schedule_srtf(processes, num_processes);
            break;
        case 4: {
            int time_quantum;
            printf("Enter time quantum for Round Robin: ");
            scanf("%d", &time_quantum);
            printf("\nExecuting Round Robin...\n");
            schedule_round_robin(processes, num_processes, time_quantum);
            break;
        }
        case 5: {
            int preemptive;
            printf("Enter 1 for preemptive or 0 for non-preemptive Priority Scheduling: ");
            scanf("%d", &preemptive);
            printf("\nExecuting Priority Scheduling...\n");
            if (preemptive) {
                schedule_priority_preemptive(processes, num_processes);
            } else {
                schedule_priority_non_preemptive(processes, num_processes);
            }
            break;
        }
        default:
            printf("Invalid choice. Exiting.\n");
            return 1;
    }

    return 0;
}