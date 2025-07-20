#include "simulator.h"
#include "../algorithms/fcfs.h"
#include "../algorithms/sjf.h"
#include "../algorithms/srtf.h"
#include "../algorithms/rr.h"  
#include "../algorithms/priority.h"
#include "../utils/process.h"
#include "../utils/queue.h"
#include "../utils/metrics.h"
#include <stdio.h>

// Initialize simulator
void init_simulator() {
    printf("CPU Scheduler Simulator initialized.\n");
}

// Run selected algorithm
void run_algorithm(int algorithm_choice, Process *processes, int process_count) {
    switch (algorithm_choice) {
        case 1:
            schedule_fcfs(processes, process_count);
            break;
        case 2:
            schedule_sjf(processes, process_count);
            break;
        case 3:
            schedule_srtf(processes, process_count);
            break;
        case 4: {
            int quantum = 2; // Default quantum
            schedule_round_robin(processes, process_count, quantum);
            break;
        }
        case 5:
            schedule_priority_non_preemptive(processes, process_count);
            break;
        default:
            printf("Invalid algorithm choice!\n");
            break;
    }
}

// Display results - placeholder
void display_results() {
    printf("Results displayed.\n");
}

// Cleanup simulator
void cleanup_simulator() {
    printf("Simulator cleaned up.\n");
}