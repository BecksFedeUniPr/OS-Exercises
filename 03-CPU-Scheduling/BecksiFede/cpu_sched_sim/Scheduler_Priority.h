#ifndef SCHEDULER_Priority_H // Should be this
#define SCHEDULER_Priority_H

#include <list>
#include <vector>
#include <iostream>
#include "Process.h"

class Scheduler_Priority
{
private:
    std::list<Process> ready_queue; // Changed from queue to list
    std::vector<Process> completed_processes;
    int current_time;
    bool is_preemptive;

    void run_preemtive() {
        while (!ready_queue.empty()) {
            auto current_proc = ready_queue.end();

            // Trova il processo con la priorità più alta tra quelli arrivati
            for (auto it = ready_queue.begin(); it != ready_queue.end(); ++it) {
                if (it->getArrivalTime() <= current_time) {
                    if (current_proc == ready_queue.end() || 
                        it->getPriority() < current_proc->getPriority()) {
                        current_proc = it;
                    } else if (it->getPriority() == current_proc->getPriority() &&
                               it->getArrivalTime() < current_proc->getArrivalTime()) {
                        current_proc = it;
                    }
                }
            }

            // Se nessun processo è pronto, avanza il tempo al prossimo arrivo
            if (current_proc == ready_queue.end()) {
                current_time++;
                continue; // Riprova a trovare un processo pronto
            }

            // Esegui il processo per 1 unità di tempo
            current_proc->setRemainingTime(current_proc->getRemainingTime() - 1);
            current_time++;

            // Se il processo è completato
            if (current_proc->getRemainingTime() == 0) {
                Process completed = *current_proc; // Crea una copia del processo
                ready_queue.erase(current_proc);   // Rimuovi il processo dalla lista

                // Calcola il turnaround time e il waiting time
                completed.setTurnaroundTime(current_time - completed.getArrivalTime());
                completed.setWaitingTime(completed.getTurnaroundTime() - completed.getBurstTime());

                // Aggiungi il processo completato alla lista
                completed_processes.push_back(completed);
            }
        }
    }

    void run_no_preemtive() {
        while (!ready_queue.empty()) {
            auto current_proc = ready_queue.end();

            // Trova il processo con la priorità più alta tra quelli arrivati
            for (auto it = ready_queue.begin(); it != ready_queue.end(); ++it) {
                if (it->getArrivalTime() <= current_time) {
                    if (current_proc == ready_queue.end() || 
                        it->getPriority() < current_proc->getPriority()) {
                        current_proc = it;
                    } else if (it->getPriority() == current_proc->getPriority() &&
                               it->getArrivalTime() < current_proc->getArrivalTime()) {
                        current_proc = it;
                    }
                }
            }

            // Se nessun processo è pronto, avanza il tempo
            if (current_proc == ready_queue.end()) {
                current_time++;
                continue; // Riprova a trovare un processo pronto
            }

            // Calcola il waiting time
            current_proc->setWaitingTime(current_time - current_proc->getArrivalTime());

            // Esegui il processo
            current_time += current_proc->getBurstTime();

            // Calcola il turnaround time
            current_proc->setTurnaroundTime(current_time - current_proc->getArrivalTime());

            // Aggiungi il processo completato alla lista
            completed_processes.push_back(*current_proc);

            // Rimuovi il processo dalla lista
            ready_queue.erase(current_proc);
        }
    }

public:
    Scheduler_Priority(const bool& is_preemtive) :  is_preemptive(is_preemtive) , current_time(0) {}

    void addProcess(Process &p)
    {
        ready_queue.push_back(p); // Changed from push to push_back
    }

    // Helper method to remove specific process
    void removeProcess(const Process &p)
    {
        ready_queue.remove(p);
    }

    bool isEmpty() const
    {
        return ready_queue.empty();
    }

    double getAverageWaitingTime() const
    {
        if (completed_processes.empty())
        {
            return 0.0;
        }

        double total_waiting = 0.0;
        for (const auto &process : completed_processes)
        {
            total_waiting += process.getWaitingTime();
        }
        return total_waiting / completed_processes.size();
    }

    double getAverageTurnaroundTime() const
    {
        if (completed_processes.empty())
        {
            return 0.0;
        }

        double total_turnaround = 0.0;
        for (const auto &process : completed_processes)
        {
            total_turnaround += process.getTurnaroundTime();
        }
        return total_turnaround / completed_processes.size();
    }

    void printMetrics() const {
        std::cout << "\n=== Scheduling Metrics ===\n";
    
        // Throughput
        double throughput = static_cast<double>(completed_processes.size()) / current_time;
        std::cout << "Throughput: " << throughput << " processes/unit time\n";
    
        // Average Turnaround Time
        double total_turnaround_time = 0.0;
        for (const auto &process : completed_processes) {
            total_turnaround_time += process.getTurnaroundTime();
        }
        double avg_turnaround_time = total_turnaround_time / completed_processes.size();
        std::cout << "Average Turnaround Time: " << avg_turnaround_time << " units\n";
    
        // Average Waiting Time
        double total_waiting_time = 0.0;
        for (const auto &process : completed_processes) {
            total_waiting_time += process.getWaitingTime();
        }
        double avg_waiting_time = total_waiting_time / completed_processes.size();
        std::cout << "Average Waiting Time: " << avg_waiting_time << " units\n";
    
        // Average Response Time
        double total_response_time = 0.0;
        for (const auto &process : completed_processes) {
            total_response_time += process.getWaitingTime(); // In RR, response time = waiting time
        }
        double avg_response_time = total_response_time / completed_processes.size();
        std::cout << "Average Response Time: " << avg_response_time << " units\n";
    
        std::cout << "============================\n\n";
    }

    void run()
    {
        if(is_preemptive)
            run_preemtive();
        else 
            run_no_preemtive();
    }

};
#endif