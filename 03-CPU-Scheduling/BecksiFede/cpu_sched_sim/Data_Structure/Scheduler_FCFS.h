#ifndef SCHEDULER_FCFS_H  // Should be this
#define SCHEDULER_FCFS_H

#include <list>
#include <vector>
#include <iostream>

#include "Process.h"

class Scheduler_FCFS
{
private:
    std::list<Process> ready_queue;      // Changed from queue to list
    std::vector<Process> completed_processes;
    int current_time;
public:
    Scheduler_FCFS() : current_time(0) {}

    void addProcess(Process &p)
    {
        ready_queue.push_back(p);        // Changed from push to push_back
    }

    // Helper method to remove specific process
    void removeProcess(const Process& p) {
        ready_queue.remove(p);
    }

    // Helper method to get first process without removing it
    Process* peekNext() {
        if (ready_queue.empty()) return nullptr;
        return &ready_queue.front();
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
    // Get next process according to FCFS policy
    Process* getNextProcess()  {
        if (ready_queue.empty()) {
            return nullptr;
        }
        
        Process* next = &ready_queue.front();
        ready_queue.pop_front();
        return next;
    }

    // Run the FCFS scheduling algorithm
    void run() {
        while (!ready_queue.empty()) {
            Process* current = getNextProcess();
            if (current != nullptr) {
                // Update process timing
                current_time += current->getBurstTime();
                current->setTurnaroundTime(current_time - current->getArrivalTime());
                current->setWaitingTime(current->getTurnaroundTime() - current->getBurstTime());
                
                // Add to completed processes
                completed_processes.push_back(*current);
            }
        }
    }
};
#endif