#ifndef PROCESS_H
#define PROCESS_H

class Process
{
private:
    const int pid;          // Process ID
    const int arrival_time; // Arrival time
    int burst_time;         // Total CPU time needed
    int priority;           // Priority level
    int waiting_time;       // Time spent waiting
    int turnaround_time;    // Total time in system
    int remaining_time;     // Remaining CPU time
    int throughput;

public:
    // Constructor
    Process(int id, int arrival, int burst, int prio = 0) : pid(id),
                                                            arrival_time(arrival),
                                                            burst_time(burst),
                                                            priority(prio),
                                                            waiting_time(0),
                                                            turnaround_time(0),
                                                            throughput(0),
                                                            remaining_time(burst) {}

    // Getters
    int getPid() const { return pid; }
    int getArrivalTime() const { return arrival_time; }
    int getBurstTime() const { return burst_time; }
    int getPriority() const { return priority; }
    int getWaitingTime() const { return waiting_time; }
    int getTurnaroundTime() const { return turnaround_time; }
    int getRemainingTime() const { return remaining_time; }
    int getThroughput() const { return throughput; } // Aggiunto qui

    // Setters (only for mutable attributes)
    void setPriority(int prio) { priority = prio; }
    void setWaitingTime(int time) { waiting_time = time; }
    void setTurnaroundTime(int time) { turnaround_time = time; }
    void setRemainingTime(int time) { remaining_time = time; }
    void setThroughput(int value) { throughput = value; } // Aggiunto qui
    void decBurstime() { --burst_time; }

    // Add after the setters and before the closing class bracket
    bool operator==(const Process &other) const
    {
        return pid == other.pid;
    }

    // Add after the operator== method
    Process &operator=(const Process &other)
    {
        if (this != &other)
        { // Evita auto-assegnazione
            // Nota: pid e arrival_time sono const e non possono essere modificati
            burst_time = other.burst_time;
            priority = other.priority;
            waiting_time = other.waiting_time;
            turnaround_time = other.turnaround_time;
            remaining_time = other.remaining_time;
            throughput = other.throughput;
        }

        return *this;
    }
};

#endif // PROCESS_H