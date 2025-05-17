#include <iostream>

#include "Process.h"
#include "QuequeProc.h"

int main()
{
    QuequeProc queque_proc;

    Process p1(0, 24,"P1"); // P1: arrival time 0, burst time 24
    Process p2(1, 3,"P2");  // P2: arrival time 1, burst time 3
    Process p3(2, 3,"P3");  // P3: arrival time 2, burst time 3

    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;

    queque_proc.addProcess(p1);
    queque_proc.addProcess(p2);
    queque_proc.addProcess(p3);

    int n_process = queque_proc.getSize();

    int time = 0;

    std::cout << "Process Queue: ";

    for (int i = 0; i < n_process; i++)
    {
        Process current_process = queque_proc[i];
        std::cout << current_process.getProcessName() << " ( Arrival: " << current_process.getArrivalTime()
                  << ", Burst: " << current_process.getBurstTime() << " ) ," ;
    }

    std::cout << std::endl << "Execution Sequence: " << std::endl;

    for (size_t i = 0; i < queque_proc.getSize(); i++)
    {
        Process& current_process = queque_proc[i];
        current_process.setStartingTime(time);
        current_process.setWaitingTime(time - current_process.getArrivalTime());
        current_process.setTurnaroundTime(current_process.getWaitingTime() + current_process.getBurstTime());
        time += current_process.getBurstTime();
        current_process.setFinishTime(time);
        avg_waiting_time += current_process.getWaitingTime();
        avg_turnaround_time += current_process.getTurnaroundTime();
    }
    

    for (int i = 0; i < n_process; i++)
    {
        Process& current_process = queque_proc[i];
        std::cout << current_process.getStartingTime() << "-" << current_process.getFinishTime() << ": "
                  << current_process.getProcessName() << " executes" << std::endl;
    }

    std::cout << "\nProcess | Turnaround Time | Waiting Time" << std::endl;
    std::cout << "--------|-----------------|-------------" << std::endl;
    for (int i = 0; i < n_process; i++)
    {
        Process& current_process = queque_proc[i];
        std::cout << current_process.getProcessName() << "\t| "
                  << current_process.getTurnaroundTime() << "\t\t| "
                  << current_process.getWaitingTime() << std::endl;
    }

    std::cout << "Average Waiting Time: " << static_cast<float>(avg_waiting_time / n_process) << std::endl;
    std::cout << "Average Turnaround Time: " << static_cast<float>(avg_turnaround_time / n_process) << std::endl;

    return 0;
}