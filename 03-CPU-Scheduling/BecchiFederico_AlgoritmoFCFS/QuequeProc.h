#ifndef QUEQUEPROC_H
#define QUEQUEPROC_H

#include "Process.h"

class QuequeProc{
    private:
        Process* queque;
        int capacity;
        int n_process;
    public:
        QuequeProc(){
            this->capacity = 0;
            this->queque = nullptr;
            this->n_process = 0;
        }

        void addProcess(const Process& proc){
            if(this->capacity == this->n_process)
                //extend queque with function TODO
            
            this->queque[this->n_process] = proc;

            this->n_process++;
        }

        Process dequee(){
            if(this->isEmpty())
                // TODO EXECPTION
            
            Process old_proc = queque[0];

            for(int i = 0 ; i < n_process - 1; ++i)
                queque[i] = queque[i + 1]; //TODO WRITE PROCESS OPERATOR CONSTRUCTOR

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