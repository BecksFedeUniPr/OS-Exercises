#ifndef SRTF_H
#define SRTF_H

#include "../utils/process.h"
#include "../utils/metrics.h"

void schedule_srtf(Process processes[], int num_processes);
int find_shortest_remaining_time(Process processes[], int num_processes, int current_time);

#endif