#include <stdio.h>
#include <stdlib.h>
#include "algorithms/fcfs.h"
#include "algorithms/sjf.h"
#include "algorithms/srtf.h"
#include "algorithms/rr.h"
#include "algorithms/priority.h"
#include "utils/process.h"

int main() {
    int choice, num_processes;
    Process processes[MAX_PROCESSES];
    
    printf("CPU Scheduling Simulator\n");
    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &num_processes);
    
    // Input process details
    for (int i = 0; i < num_processes; i++) {
        printf("Process %d:\n", i + 1);
        processes[i].id = i + 1;
        printf("  Arrival time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("  Burst time: ");
        scanf("%d", &processes[i].burst_time);
        printf("  Priority: ");
        scanf("%d", &processes[i].priority);
        
        // Initialize other fields
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = 0;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].response_time = 0;
        processes[i].started = 0;
    }
    
    printf("\nSelect Scheduling Algorithm:\n");
    printf("1. FCFS (First-Come First-Served)\n");
    printf("2. SJF (Shortest Job First)\n");
    printf("3. SRTF (Shortest Remaining Time First)\n");
    printf("4. RR (Round Robin)\n");
    printf("5. Priority Scheduling\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            schedule_fcfs(processes, num_processes);
            break;
        case 2:
            schedule_sjf(processes, num_processes);
            break;
        case 3:
            schedule_srtf(processes, num_processes);
            break;
        case 4: {
            int quantum;
            printf("Enter time quantum: ");
            scanf("%d", &quantum);
            schedule_round_robin(processes, num_processes, quantum);
            break;
        }
        case 5: {
            int preemptive;
            printf("Preemptive (1) or Non-preemptive (0): ");
            scanf("%d", &preemptive);
            if (preemptive) {
                schedule_priority_preemptive(processes, num_processes);
            } else {
                schedule_priority_non_preemptive(processes, num_processes);
            }
            break;
        }
        default:
            printf("Invalid choice. Please select a valid option.\n");
            break;
    }

    return 0;
}