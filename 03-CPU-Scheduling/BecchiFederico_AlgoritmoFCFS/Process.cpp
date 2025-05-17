#include "Process.h"

Process::Process() {
    this->arrival_time = 0;
    this->burst_time = 0;
    this->turnaround_time = 0;
    this->waiting_time = 0;
    this->starting_time = 0;
    this->finish_time = 0;
    this->process_name = "Process"; 
}

Process::Process(time_unit arv_time, time_unit brst_time,std::string name)
    : arrival_time(arv_time), burst_time(brst_time), turnaround_time(0), waiting_time(0) , 
      starting_time(0), finish_time(0), process_name(name) {
}

Process& Process::operator=(const Process& other) {
    if (this != &other) {
        this->arrival_time = other.arrival_time;
        this->burst_time = other.burst_time;
        this->turnaround_time = other.turnaround_time;
        this->waiting_time = other.waiting_time;
        this->starting_time = other.starting_time;
        this->finish_time = other.finish_time;
        this->process_name = other.process_name; // My choise to copy the process name should be used an id
    }
    return *this;
}

void Process::setArrivalTime(time_unit arv_time) {
    this->arrival_time = arv_time;
}

void Process::setBurstTime(time_unit brst_time) {
    this->burst_time = brst_time;
}

void Process::setTurnaroundTime(time_unit trnd_time) {
    this->turnaround_time = trnd_time;
}

void Process::setWaitingTime(time_unit w_time) {
    this->waiting_time = w_time;
}

void Process::setProcessName(const std::string& name) {
    this->process_name = name;
}

void Process::setStartingTime(time_unit st_time) {
    this->starting_time = st_time;
}
void Process::setFinishTime(time_unit f_time) {
    this->finish_time = f_time;
}

Process::time_unit Process::getArrivalTime() const {
    return this->arrival_time;
}

Process::time_unit Process::getBurstTime() const {
    return this->burst_time;
}

Process::time_unit Process::getTurnaroundTime() const {
    return this->turnaround_time;
}

Process::time_unit Process::getWaitingTime() const {
    return this->waiting_time;
}

std::string Process::getProcessName() const {
    return this->process_name;
}

Process::time_unit Process::getStartingTime() const {
    return this->starting_time;
}
Process::time_unit Process::getFinishTime() const {
    return this->finish_time;
}
