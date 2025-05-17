#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
public:
    using time_unit = int;
    
    Process();
    Process(time_unit arv_time, time_unit brst_time,std::string name);
    ~Process() = default;
    Process(const Process& other) = default;
    Process& operator=(const Process& other);
    
    void setArrivalTime(time_unit arv_time);
    void setBurstTime(time_unit brst_time);
    void setTurnaroundTime(time_unit trnd_time);
    void setWaitingTime(time_unit w_time);
    void setProcessName(const std::string& name);
    void setStartingTime(time_unit st_time);
    void setFinishTime(time_unit f_time);
    
    time_unit getArrivalTime() const;
    time_unit getBurstTime() const;
    time_unit getTurnaroundTime() const;
    time_unit getWaitingTime() const;
    time_unit getStartingTime() const;
    time_unit getFinishTime() const;
    std::string getProcessName() const;

    
private:
    time_unit arrival_time;
    time_unit burst_time;
    time_unit turnaround_time;
    time_unit waiting_time;
    time_unit starting_time;
    time_unit finish_time;
    std::string process_name;
};

#endif