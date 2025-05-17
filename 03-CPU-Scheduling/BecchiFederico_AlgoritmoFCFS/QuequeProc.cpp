#include "QuequeProc.h"

void QuequeProc::extend_queue() {
    int new_capacity = this->capacity * 2;
    Process* new_queue = new Process[new_capacity];
    
    for (int i = 0; i < n_process; i++) {
        new_queue[i] = queque[i];
    }
    
    delete[] queque;
    queque = new_queue;
    capacity = new_capacity;
}

QuequeProc::QuequeProc() {
    this->capacity = 10;
    this->queque = new Process[this->capacity];
    this->n_process = 0;
}

void QuequeProc::addProcess(const Process& proc) {
    if (this->capacity == this->n_process)
        this->extend_queue();
    
    this->queque[this->n_process] = proc;
    this->n_process++;
}

Process QuequeProc::dequee() {
    if (this->isEmpty()) {
        throw "BUCATO"; // TODO: Usare un'eccezione appropriata
    }
    
    Process old_proc = queque[0];
    
    for (int i = 0; i < n_process - 1; i++)
        queque[i] = queque[i + 1];
    
    n_process--;
    
    return old_proc;
}

int QuequeProc::getSize() {
    return this->n_process;
}

bool QuequeProc::isEmpty() {
    return getSize() == 0;
}

const Process& QuequeProc::operator[](int index) const {
    if(index < 0 || index >= n_process) {
        throw "Error index"; // TODO: Usare un'eccezione appropriata
    }
    return queque[index];
}

QuequeProc::~QuequeProc() {
    for(int i = 0; i < n_process; i++) {
        queque[i].~Process();
    }
    
    delete[] queque;
}