#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "../utils/process.h"
#include "../utils/queue.h"
#include "../utils/metrics.h"

void schedule_round_robin(Process processes[], int num_processes, int time_quantum);

#endif