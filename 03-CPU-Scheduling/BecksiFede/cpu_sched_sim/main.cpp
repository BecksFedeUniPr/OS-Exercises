#include <iostream>
#include "Process.h"
#include "Scheduler_Priority.h"

int main() {
    // Crea uno scheduler basato sulla priorità
    Scheduler_Priority scheduler(true);

    // Crea i processi di test
    Process p1(1, 0, 10, 3);  // P1: arrival=0, burst=10, priority=3
    Process p2(2, 0, 5, 1);   // P2: arrival=0, burst=5, priority=1
    Process p3(3, 0, 8, 4);   // P3: arrival=0, burst=8, priority=4
    Process p4(4, 0, 2, 2);   // P4: arrival=0, burst=2, priority=2

    // Aggiungi i processi allo scheduler
    scheduler.addProcess(p1);
    scheduler.addProcess(p2);
    scheduler.addProcess(p3);
    scheduler.addProcess(p4);

    std::cout << "Starting Priority Scheduling Simulation...\n";

    // Esegui lo scheduler
    scheduler.run();

    // Stampa le statistiche finali
    scheduler.printMetrics();

    return 0;
}