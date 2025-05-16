# Threads & Multithreading

## Contents
1. [Thread Fundamentals](#thread-fundamentals)
2. [Creating Threads](#creating-threads)
3. [Thread Synchronization](#thread-synchronization)
4. [Thread Termination](#thread-termination)
5. [Common Thread Patterns](#common-thread-patterns)

## Thread Fundamentals

Threads are lightweight execution units that share the same memory space within a process.

### Key Concepts
- **Process vs Thread**: A process has its own address space; threads share their process's address space
- **Concurrency**: Multiple threads execute "simultaneously" (may be actual parallelism on multicore systems)
- **Shared Resources**: All threads share heap memory, file descriptors, and global variables
- **Thread-specific Data**: Each thread has its own stack and registers

### Thread Benefits
- Lower overhead compared to processes
- Simplified data sharing through common memory
- Faster context switching
- Better resource utilization

## Creating Threads

The pthread library provides functions for thread creation and management.

```C
pthread_t thread;
```

### Basic Thread Creation
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thread_function(void* arg) {
    int* value = (int*)arg;
    printf("Thread received value: %d\n", *value);
    
    // Thread return value (can be accessed by pthread_join)
    int* result = malloc(sizeof(int));
    *result = (*value) * 2;
    return result;
}

int main() {
    pthread_t thread;
    int value = 42;
    
    // Create thread
    int status = pthread_create(&thread, NULL, thread_function, &value);
    if (status != 0) {
        printf("Error creating thread: %d\n", status);
        return 1;
    }
    
    // Wait for thread to complete
    void* thread_result;
    pthread_join(thread, &thread_result);
    
    // Process result
    printf("Thread returned: %d\n", *(int*)thread_result);
    free(thread_result);
    
    return 0;
}
```

### Thread Attributes
```c
pthread_attr_t attr;
pthread_attr_init(&attr);

// Set stack size (in bytes)
pthread_attr_setstacksize(&attr, 2097152);  // 2MB stack

// Set thread as detached
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

// Create thread with attributes
pthread_t thread;
pthread_create(&thread, &attr, thread_function, arg);

// Clean up attribute
pthread_attr_destroy(&attr);
```

## Thread Synchronization

Thread synchronization is crucial to prevent race conditions when accessing shared resources.

### Mutex (Mutual Exclusion)
```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

void* increment_counter(void* arg) {
    int iterations = 10000;
    
    for (int i = 0; i < iterations; i++) {
        // Lock the mutex before accessing shared resource
        pthread_mutex_lock(&mutex);
        
        // Critical section
        counter++;
        
        // Unlock the mutex
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    
    pthread_create(&thread1, NULL, increment_counter, NULL);
    pthread_create(&thread2, NULL, increment_counter, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Final counter value: %d\n", counter);
    
    // Destroy mutex when done
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
```

### Condition Variables
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int data_ready = 0;

void* producer(void* arg) {
    printf("Producer: Working on data...\n");
    sleep(2);  // Simulate work
    
    pthread_mutex_lock(&mutex);
    
    // Set the flag and signal consumer
    data_ready = 1;
    printf("Producer: Data ready, signaling consumer\n");
    pthread_cond_signal(&condition);
    
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* consumer(void* arg) {
    printf("Consumer: Waiting for data...\n");
    
    pthread_mutex_lock(&mutex);
    
    while (!data_ready) {
        // Wait for signal (releases mutex while waiting)
        pthread_cond_wait(&condition, &mutex);
    }
    
    printf("Consumer: Received signal, processing data\n");
    // Process data here
    
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    pthread_create(&producer_thread, NULL, producer, NULL);
    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
    
    return 0;
}
```

### Semaphores
```c
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_ITEMS 5
#define BUFFER_SIZE 5

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

void* producer(void* arg) {
    int item;
    for (int i = 0; i < MAX_ITEMS; i++) {
        item = i;  // Produce item
        
        sem_wait(&empty);  // Wait if buffer is full
        pthread_mutex_lock(&mutex);
        
        // Critical section
        buffer[in] = item;
        printf("Producer: Inserted item %d at position %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&full);  // Signal that buffer has an item
        
        sleep(1);  // Simulate work
    }
    return NULL;
}

void* consumer(void* arg) {
    int item;
    for (int i = 0; i < MAX_ITEMS; i++) {
        sem_wait(&full);  // Wait if buffer is empty
        pthread_mutex_lock(&mutex);
        
        // Critical section
        item = buffer[out];
        printf("Consumer: Removed item %d from position %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);  // Signal that buffer has space
        
        sleep(2);  // Simulate work
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    
    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE);  // Buffer is initially empty
    sem_init(&full, 0, 0);            // No items to consume initially
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    // Clean up resources
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
```

## Thread Termination

Proper thread termination is crucial to prevent memory leaks and ensure clean program execution.

### Joining Threads
```c
pthread_t thread;
pthread_create(&thread, NULL, thread_function, arg);

// Wait for thread to complete and get result
void* result;
int status = pthread_join(thread, &result);
if (status != 0) {
    printf("Error joining thread: %d\n", status);
}

// Don't forget to free any allocated memory
free(result);
```

### Detached Threads
```c
pthread_t thread;
pthread_create(&thread, NULL, thread_function, arg);

// Detach thread (resources automatically released when thread exits)
pthread_detach(thread);

// No need to call pthread_join
```

### Thread Cancellation
```c
void* thread_function(void* arg) {
    // Setup cancellation state
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    
    while (1) {
        // Cancellation point
        pthread_testcancel();
        
        // Do some work
        printf("Thread working...\n");
        sleep(1);
    }
    
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    
    sleep(5);  // Let thread run for 5 seconds
    
    // Cancel the thread
    pthread_cancel(thread);
    
    // Wait for thread to be cancelled
    pthread_join(thread, NULL);
    
    printf("Thread cancelled and joined\n");
    return 0;
}
```

## Common Thread Patterns

### Problem Description
- Producers generate data items and place them in a bounded buffer
- Consumers remove data items from the buffer for processing
- The buffer has limited capacity
- Challenge: Coordinate access to prevent buffer overflow/underflow and ensure data integrity

### Key Synchronization Requirements
- Producers must wait if the buffer is full
- Consumers must wait if the buffer is empty
- Only one process can access the buffer at a time

### Producer-Consumer Pattern
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} BoundedBuffer;

BoundedBuffer shared_buffer = {
    .in = 0,
    .out = 0,
    .count = 0
};

void init_buffer(BoundedBuffer* buffer) {
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->not_full, NULL);
    pthread_cond_init(&buffer->not_empty, NULL);
}

void put(BoundedBuffer* buffer, int item) {
    pthread_mutex_lock(&buffer->mutex);
    
    // Wait until buffer is not full
    while (buffer->count == BUFFER_SIZE) {
        pthread_cond_wait(&buffer->not_full, &buffer->mutex);
    }
    
    // Add item to buffer
    buffer->buffer[buffer->in] = item;
    buffer->in = (buffer->in + 1) % BUFFER_SIZE;
    buffer->count++;
    
    printf("Producer: Added item %d, buffer count: %d\n", item, buffer->count);
    
    // Signal that buffer is not empty
    pthread_cond_signal(&buffer->not_empty);
    pthread_mutex_unlock(&buffer->mutex);
}

int get(BoundedBuffer* buffer) {
    pthread_mutex_lock(&buffer->mutex);
    
    // Wait until buffer is not empty
    while (buffer->count == 0) {
        pthread_cond_wait(&buffer->not_empty, &buffer->mutex);
    }
    
    // Remove item from buffer
    int item = buffer->buffer[buffer->out];
    buffer->out = (buffer->out + 1) % BUFFER_SIZE;
    buffer->count--;
    
    printf("Consumer: Removed item %d, buffer count: %d\n", item, buffer->count);
    
    // Signal that buffer is not full
    pthread_cond_signal(&buffer->not_full);
    pthread_mutex_unlock(&buffer->mutex);
    
    return item;
}

void* producer(void* arg) {
    int num_items = 10;
    
    for (int i = 0; i < num_items; i++) {
        put(&shared_buffer, i);
        usleep(rand() % 500000);  // Sleep between 0-500ms
    }
    
    return NULL;
}

void* consumer(void* arg) {
    int num_items = 10;
    
    for (int i = 0; i < num_items; i++) {
        int item = get(&shared_buffer);
        usleep(rand() % 800000);  // Sleep between 0-800ms
    }
    
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t producer_thread, consumer_thread;
    
    init_buffer(&shared_buffer);
    
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    pthread_mutex_destroy(&shared_buffer.mutex);
    pthread_cond_destroy(&shared_buffer.not_full);
    pthread_cond_destroy(&shared_buffer.not_empty);
    
    return 0;
}
```

## Exercises

1. Create a multithreaded application that calculates the sum of elements in an array in parallel
2. Implement a producer-consumer pattern using condition variables
3. Write a thread-safe data structure (e.g., linked list or queue)
4. Create a program that demonstrates thread priorities
5. Implement a barrier synchronization mechanism using pthreads

