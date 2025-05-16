# Critical Section Problems and Solutions

This section covers various synchronization mechanisms to solve critical section problems in concurrent programming:

## Contents
1. [Mutex Locks](#mutex-locks)
2. [Read/Write Locks](#readwrite-locks)
3. [Monitors](#monitors)
4. [Binary Semaphores](#binary-semaphores)
5. [Dining Philosophers Problem](#dining-philosophers-problem)

## Mutex Locks

Mutex (Mutual Exclusion) locks are the simplest synchronization tools used to protect critical sections.

### Key Concepts
- A mutex has two states: locked and unlocked
- Only one thread can hold the lock at any time
- Protects critical sections from concurrent access

### Basic Operations
- `lock()`: Acquires the mutex (blocks if already locked)
- `unlock()`: Releases the mutex, allowing other threads to acquire it

### Example Implementation
```c
// Basic mutex operations
mutex_lock(&mutex);    // Enter critical section
// Critical section code
mutex_unlock(&mutex);  // Exit critical section
```

## Read/Write Locks

Read/Write locks allow concurrent reads but exclusive writes.

### Key Concepts
- Multiple readers can access simultaneously
- Writers have exclusive access
- Prioritization policies may vary (reader vs. writer preference)

### Basic Operations
- `read_lock()`: Acquires a read lock (blocks if write lock is held)
- `read_unlock()`: Releases a read lock
- `write_lock()`: Acquires a write lock (blocks if any lock is held)
- `write_unlock()`: Releases a write lock

### Example Implementation
```c
// Reader code
read_lock(&rwlock);
// Read shared data
read_unlock(&rwlock);

// Writer code
write_lock(&rwlock);
// Modify shared data
write_unlock(&rwlock);
```

## Monitors

Monitors are high-level synchronization constructs that encapsulate data and operations.

### Key Concepts
- Encapsulation of shared data and operations
- Mutual exclusion is automatic
- Condition variables for thread coordination

### Components
- Shared data variables
- Procedures that operate on the data
- Condition variables for synchronization

### Example Implementation
```java
// Monitor implementation in Java
public class BoundedBuffer {
    private final Object[] buffer = new Object[BUFFER_SIZE];
    private int count = 0, in = 0, out = 0;

    public synchronized void put(Object item) throws InterruptedException {
        while (count == BUFFER_SIZE)
            wait();  // Buffer full, wait
        
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        
        notify();  // Wake up consumer
    }

    public synchronized Object get() throws InterruptedException {
        while (count == 0)
            wait();  // Buffer empty, wait
        
        Object item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        
        notify();  // Wake up producer
        return item;
    }
}
```

## Binary Semaphores

Binary semaphores are synchronization primitives with values 0 or 1.

### Key Concepts
- Implemented without busy waiting
- Can be used for signaling and mutual exclusion
- Backed by OS-level blocking mechanisms

### Basic Operations
- `wait(S)` or `P(S)`: Decrements the semaphore value (blocks if value is 0)
- `signal(S)` or `V(S)`: Increments the semaphore value (wakes up waiting thread)

### Example Implementation
```c
// Using binary semaphores for mutual exclusion
semaphore mutex = 1;  // Initialize to 1

// Thread code
wait(mutex);          // Enter critical section
// Critical section code
signal(mutex);        // Exit critical section

// Signaling between threads
semaphore flag = 0;   // Initialize to 0

// Thread A
// Do work
signal(flag);         // Signal Thread B

// Thread B
wait(flag);           // Wait for Thread A
// Continue execution
```

## Dining Philosophers Problem

A classic synchronization problem demonstrating deadlock and resource allocation.

### Problem Description
- Five philosophers sit around a circular table
- Each philosopher alternates between thinking and eating
- To eat, a philosopher needs two forks (shared with adjacent philosophers)
- Challenge: Prevent deadlock and starvation

### Solutions

#### Solution 1: Resource Hierarchy
```c
#define N 5  // Number of philosophers

void philosopher(int i) {
    while(true) {
        think();
        
        // Pick up forks in order (lower index first)
        if (i == N-1) {
            pickup_fork((i+1) % N);  // Pick up left fork first
            pickup_fork(i);          // Then right fork
        } else {
            pickup_fork(i);          // Pick up right fork first
            pickup_fork((i+1) % N);  // Then left fork
        }
        
        eat();
        
        // Put down forks
        putdown_fork(i);
        putdown_fork((i+1) % N);
    }
}
```

#### Solution 2: Using Semaphores
```c
semaphore fork[N] = {1, 1, 1, 1, 1};
semaphore room = 4;  // Allow only 4 philosophers to attempt eating

void philosopher(int i) {
    while(true) {
        think();
        
        wait(room);              // Enter the room
        wait(fork[i]);           // Pick up right fork
        wait(fork[(i+1) % N]);   // Pick up left fork
        
        eat();
        
        signal(fork[i]);         // Put down right fork
        signal(fork[(i+1) % N]); // Put down left fork
        signal(room);            // Leave the room
    }
}
```

## Exercises

1. Implement a thread-safe queue using mutex locks
2. Create a reader-writer solution with writer priority
3. Implement a monitor-based solution for the producer-consumer problem
4. Solve the dining philosophers problem using monitors
5. Compare the performance of different synchronization mechanisms in a multi-threaded environment