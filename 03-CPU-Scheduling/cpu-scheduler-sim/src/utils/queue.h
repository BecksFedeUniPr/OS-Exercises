#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"
#include "../../include/common.h"

typedef struct {
    Process* processes[MAX_PROCESSES];
    int front;
    int rear;
    int size;
} Queue;

void init_queue(Queue* q);
int is_empty(Queue* q);
int is_full(Queue* q);
void enqueue(Queue* q, Process* p);
Process* dequeue(Queue* q);
Process* peek(Queue* q);
void print_queue(Queue* q);

#endif