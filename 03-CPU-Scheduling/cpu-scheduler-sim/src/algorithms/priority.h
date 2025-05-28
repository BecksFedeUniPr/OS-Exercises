#ifndef PRIORITY_H
#define PRIORITY_H

#include "../utils/process.h"
#include "../utils/metrics.h"

void schedule_priority_preemptive(Process processes[], int num_processes);
void schedule_priority_non_preemptive(Process processes[], int num_processes);
int find_highest_priority(Process processes[], int num_processes, int current_time, int preemptive);

#endif