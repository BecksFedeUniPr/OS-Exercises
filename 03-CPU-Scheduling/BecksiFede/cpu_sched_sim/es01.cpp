#include <iostream>
#include "Data_Structure/Process.h"
#include "Data_Structure/Scheduler_FCFS.h"
#include "Data_Structure/Scheduler_SJF.h"
#include "Data_Structure/Scheduler_RR.h"

void runTests() {
    // Definizione dei processi
    std::vector<Process> processes = {
        Process(1, 0, 10), // P1: arrival=0, burst=10
        Process(2, 0, 5),  // P2: arrival=0, burst=5
        Process(3, 0, 8),  // P3: arrival=0, burst=8
        Process(4, 0, 2)   // P4: arrival=0, burst=2
    };

    // Test FCFS
    std::cout << "=== FCFS ===\n";
    Scheduler_FCFS fcfs_scheduler;
    for (auto &process : processes) {
        fcfs_scheduler.addProcess(process);
    }
    fcfs_scheduler.run();
    fcfs_scheduler.printMetrics();

    // Test SJF
    std::cout << "=== SJF ===\n";
    Scheduler_SJF sjf_scheduler;
    for (auto &process : processes) {
        sjf_scheduler.addProcess(process);
    }
    sjf_scheduler.run();
    sjf_scheduler.printMetrics();

    // Test RR con quantum = 2
    std::cout << "=== RR (Quantum = 2) ===\n";
    Scheduler_RR rr_scheduler(2);
    for (auto &process : processes) {
        rr_scheduler.addProcess(process);
    }
    rr_scheduler.run();
    rr_scheduler.printMetrics();
}

int main() {
    runTests();
    return 0;
}