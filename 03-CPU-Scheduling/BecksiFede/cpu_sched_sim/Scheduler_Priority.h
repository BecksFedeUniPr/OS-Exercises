#ifndef SCHEDULER_SJF_H // Should be this
#define SCHEDULER_SJF_H

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

public:
    Scheduler_Priority(const int& quantum_time) : current_time(0) {}

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
        while (!ready_queue.empty())
        {
        
    
        
        }
    }
};
#endif