#ifndef PROCESS_H
#define PROCESS_H

class Process{
    private:

        using time_unit = int; // TODO Clock Cycle <1s ?  

        const time_unit arrival_time;
        const time_unit burst_time;
        time_unit turnaround_time;
        time_unit waiting_time;

    public:
        Process(time_unit arv_time , time_unit brst_time) : arrival_time(arv_time) , burst_time(brst_time){
            this->turnaround_time = 0;
            this->waiting_time = 0;
        }

        //TODO getter and setter for needs

        Process& operator=(const Process& other) {
            if (this != &other) {
                Process* new_process = new Process(other.arrival_time,other.burst_time);

                this->turnaround_time = other.turnaround_time;
                this->waiting_time = other.waiting_time;

                return *new_process;
            }
            return *this;
        }
};

#endif