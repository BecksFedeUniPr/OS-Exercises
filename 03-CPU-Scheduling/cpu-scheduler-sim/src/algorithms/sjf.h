#ifndef SJF_H
#define SJF_H

#include "../utils/process.h"
#include "../utils/metrics.h"

void schedule_sjf(Process processes[], int num_processes);
int find_shortest_job(Process processes[], int num_processes, int current_time);

#endif