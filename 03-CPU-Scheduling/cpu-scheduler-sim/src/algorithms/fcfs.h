#ifndef FCFS_H
#define FCFS_H

#include "../utils/process.h"

// Function to schedule processes using the FCFS algorithm
void schedule_fcfs(Process processes[], int num_processes);

// Function to calculate and display metrics for FCFS scheduling
void calculate_fcfs_metrics(Process processes[], int num_processes);

#endif // FCFS_H