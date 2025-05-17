#include <iostream>

#include "Process.h"
#include "QuequeProc.h"



int main(){
    QuequeProc queque_proc;

    Process p1(0, 24);  // P1: arrival time 0, burst time 24
    Process p2(1, 3);   // P2: arrival time 1, burst time 3
    Process p3(2, 3); // P3: arrival time 2, burst time 3

    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;
    
    queque_proc.addProcess(p1);
    queque_proc.addProcess(p2);
    queque_proc.addProcess(p3);

    int n_process = queque_proc.getSize();

    int time = 0;

    while (!queque_proc.isEmpty())
    {
        Process current_process = queque_proc.dequee();
        std::cout << "Processing: , Burst Time: " << current_process.getBurstTime() << std::endl;
        
        current_process.setWaitingTime(current_process.getArrivalTime() - time);
        current_process.setTurnaroundTime(current_process.getWaitingTime() + current_process.getBurstTime());
        time += current_process.getBurstTime();
        avg_waiting_time += current_process.getWaitingTime();
        avg_turnaround_time += current_process.getTurnaroundTime();
    }

    std::cout << "Average Waiting Time: " << avg_waiting_time / n_process << std::endl;
    std::cout << "Average Turnaround Time: " << avg_turnaround_time / n_process << std::endl;
    
}