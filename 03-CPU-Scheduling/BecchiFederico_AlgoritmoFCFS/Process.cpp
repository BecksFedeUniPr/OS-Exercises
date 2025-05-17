#include "Process.h"

Process::Process() {
    this->arrival_time = 0;
    this->burst_time = 0;
    this->turnaround_time = 0;
    this->waiting_time = 0;
}

Process::Process(time_unit arv_time, time_unit brst_time)
    : arrival_time(arv_time), burst_time(brst_time), turnaround_time(0), waiting_time(0)
{
}

Process& Process::operator=(const Process& other) {
    if (this != &other) {
        this->arrival_time = other.arrival_time;
        this->burst_time = other.burst_time;
        this->turnaround_time = other.turnaround_time;
        this->waiting_time = other.waiting_time;
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