#ifndef QUEQUEPROC_H
#define QUEQUEPROC_H

#include "Process.h"

class QuequeProc {
private:
    Process* queque;
    int capacity;
    int n_process;

    void extend_queue();
public:
    QuequeProc();
    ~QuequeProc();

    void addProcess(const Process& proc);
    Process dequee();
    int getSize();
    bool isEmpty();
    const Process& operator[](int index) const;
};

#endif