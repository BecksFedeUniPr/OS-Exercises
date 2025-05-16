#ifndef QUEQUEPROC_H
#define QUEQUEPROC_H

#include "Process.h"

class QuequeProc{
    private:
        Process* queque;
        int capacity;
        int n_process;

        void extend_queue() {
            int new_capacity = this->capacity * 2;
            Process* new_queue = new Process[new_capacity];
            
            for(int i = 0; i < n_process; i++) {
                new_queue[i] = queque[i];
            }
            
            delete[] queque;
            queque = new_queue;
            capacity = new_capacity;
        }
    public:
        QuequeProc(){
            this->capacity = 10;
            this->queque = new Process[this->capacity];
            this->n_process = 0;
        }

        void addProcess(const Process& proc){
            if(this->capacity == this->n_process)
                this->extend_queue();
            
            this->queque[this->n_process] = proc;

            this->n_process++;
        }

        Process dequee(){
            if(this->isEmpty()){
                throw "BUCATO"; // TODO CORRECT EXCPTION
            }
            
            Process old_proc = queque[0];

            for(int i = 0 ; i < n_process - 1; ++i)
                queque[i] = queque[i + 1];

            n_process--;

            return old_proc;
        }

        int getSize(){
            return this->n_process;
        }

        bool isEmpty(){
            return getSize() == 0;
        }

};

#endif