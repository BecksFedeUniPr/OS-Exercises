#include "queue.h"

void init_queue(Queue* q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

int is_empty(Queue* q) {
    return q->size == 0;
}

int is_full(Queue* q) {
    return q->size == MAX_PROCESSES;
}

void enqueue(Queue* q, Process* p) {
    if (!is_full(q)) {
        q->rear = (q->rear + 1) % MAX_PROCESSES;
        q->processes[q->rear] = p;
        q->size++;
    }
}

Process* dequeue(Queue* q) {
    if (!is_empty(q)) {
        Process* p = q->processes[q->front];
        q->front = (q->front + 1) % MAX_PROCESSES;
        q->size--;
        return p;
    }
    return NULL;
}

Process* peek(Queue* q) {
    if (!is_empty(q)) {
        return q->processes[q->front];
    }
    return NULL;
}

void print_queue(Queue* q) {
    printf("Queue: ");
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % MAX_PROCESSES;
        printf("P%d ", q->processes[index]->id);
    }
    printf("\n");
}