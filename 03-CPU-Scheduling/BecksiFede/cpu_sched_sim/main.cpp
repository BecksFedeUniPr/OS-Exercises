#include <iostream>
#include "Process.h"
#include "Scheduler_RR.h"

int main() {
    // Crea uno scheduler Round Robin con time quantum = 2
    Scheduler_RR scheduler(2);

    // Crea i processi di test
    Process p1(1, 0, 10);  // P1: arrival=0, burst=10
    Process p2(2, 0, 5);   // P2: arrival=0, burst=5
    Process p3(3, 0, 8);   // P3: arrival=0, burst=8

    // Aggiungi i processi allo scheduler
    scheduler.addProcess(p1);
    scheduler.addProcess(p2);
    scheduler.addProcess(p3);

    std::cout << "Starting Round Robin Scheduling Simulation...\n";

    // Esegui lo scheduler
    scheduler.run();

    // Stampa le statistiche finali
    scheduler.printStatistics();

    return 0;
}