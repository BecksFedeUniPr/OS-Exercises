#ifndef SIMULATOR_H

#define SIMULATOR_H

#include "../utils/process.h"
#include "../utils/queue.h"

// Function to initialize the simulator
void init_simulator();

// Function to run the selected scheduling algorithm
void run_algorithm(int algorithm_choice, Process *processes, int process_count);

// Function to display the results of the scheduling
void display_results();

// Function to cleanup resources used by the simulator
void cleanup_simulator();

#endif // SIMULATOR_H