# Disk Scheduling Algorithms

This section covers various disk scheduling algorithms used in operating systems to manage disk I/O requests:

## Contents
1. [Disk Scheduling Fundamentals](#disk-scheduling-fundamentals)
2. [FCFS (First-Come First-Served)](#fcfs-first-come-first-served)
3. [SCAN (Elevator Algorithm)](#scan-elevator-algorithm)
4. [C-SCAN (Circular SCAN)](#c-scan-circular-scan)
5. [Performance Comparison](#performance-comparison)

## Disk Scheduling Fundamentals

Disk scheduling algorithms determine the order in which disk I/O requests are serviced to minimize seek time and maximize throughput.

### Key Concepts
- **Seek Time**: Time taken to position the disk arm at the desired track
- **Rotational Latency**: Time waiting for the disk to rotate to the desired sector
- **Transfer Time**: Time to read/write data once positioned
- **Disk Access Time** = Seek Time + Rotational Latency + Transfer Time
- **Disk Arm Movement**: The physical movement of the read/write head
- **Seek Distance**: Number of tracks the disk arm must move

## FCFS (First-Come First-Served)

The simplest disk scheduling algorithm that services requests in the order they arrive.

### Key Concepts
- Requests are processed in the exact order they are received
- Fair but potentially inefficient
- No starvation (every request is eventually serviced)

### Advantages and Disadvantages
- **Advantages**: Simple implementation, fair allocation
- **Disadvantages**: Can lead to long seek times, no optimization for seek distance

### Example Implementation
```c
// FCFS disk scheduling implementation
typedef struct {
    int request_queue[MAX_REQUESTS];
    int queue_size;
    int current_position;
} DiskScheduler;

void initialize(DiskScheduler *scheduler, int start_position) {
    scheduler->queue_size = 0;
    scheduler->current_position = start_position;
}

void addRequest(DiskScheduler *scheduler, int track) {
    if (scheduler->queue_size < MAX_REQUESTS) {
        scheduler->request_queue[scheduler->queue_size++] = track;
    }
}

int calculateSeekTime(DiskScheduler *scheduler) {
    int total_seek_time = 0;
    int current_position = scheduler->current_position;
    
    for (int i = 0; i < scheduler->queue_size; i++) {
        // Calculate absolute distance between current position and request
        total_seek_time += abs(scheduler->request_queue[i] - current_position);
        current_position = scheduler->request_queue[i];
    }
    
    scheduler->current_position = current_position;
    return total_seek_time;
}
```

### FCFS Example Execution
```
Initial head position: 50
Request Queue: 95, 180, 34, 119, 11, 123, 62, 64

Sequence of head movement:
50 → 95 (seek distance: 45)
95 → 180 (seek distance: 85)
180 → 34 (seek distance: 146)
34 → 119 (seek distance: 85)
119 → 11 (seek distance: 108)
11 → 123 (seek distance: 112)
123 → 62 (seek distance: 61)
62 → 64 (seek distance: 2)

Total seek distance: 644
Average seek distance: 80.5
```

## SCAN (Elevator Algorithm)

SCAN moves the disk arm in one direction servicing requests until it reaches the end, then reverses direction.

### Key Concepts
- Also known as the elevator algorithm
- Disk arm moves in one direction until no more requests in that direction
- Then reverses direction and services requests in the opposite direction
- Reduces maximum waiting time compared to FCFS

### Advantages and Disadvantages
- **Advantages**: More efficient than FCFS, avoids starvation
- **Disadvantages**: Requests at recently visited locations may have longer wait times

### Example Implementation
```c
// SCAN disk scheduling implementation
typedef struct {
    int request_queue[MAX_REQUESTS];
    int queue_size;
    int current_position;
    bool direction_up;  // true = moving toward higher tracks
} ScanScheduler;

void initialize(ScanScheduler *scheduler, int start_position, bool direction_up) {
    scheduler->queue_size = 0;
    scheduler->current_position = start_position;
    scheduler->direction_up = direction_up;
}

// Add all requests to the queue
void addRequest(ScanScheduler *scheduler, int track) {
    if (scheduler->queue_size < MAX_REQUESTS) {
        scheduler->request_queue[scheduler->queue_size++] = track;
    }
}

int calculateSeekTime(ScanScheduler *scheduler, int disk_size) {
    int total_seek_time = 0;
    int current_position = scheduler->current_position;
    bool direction_up = scheduler->direction_up;
    
    // Sort requests for easier processing
    int sorted_queue[MAX_REQUESTS];
    for (int i = 0; i < scheduler->queue_size; i++) {
        sorted_queue[i] = scheduler->request_queue[i];
    }
    qsort(sorted_queue, scheduler->queue_size, sizeof(int), compare);
    
    // Find position where current head is relative to requests
    int current_index = 0;
    while (current_index < scheduler->queue_size && 
           sorted_queue[current_index] < current_position) {
        current_index++;
    }
    
    // Process requests based on direction
    if (direction_up) {
        // First service all requests above current position
        for (int i = current_index; i < scheduler->queue_size; i++) {
            total_seek_time += abs(sorted_queue[i] - current_position);
            current_position = sorted_queue[i];
        }
        
        // If we reach the end, change direction and service remaining requests
        if (current_index > 0) {
            // Go to the end of the disk
            total_seek_time += abs(disk_size - 1 - current_position);
            current_position = disk_size - 1;
            
            // Then move back, servicing the remaining requests
            for (int i = current_index - 1; i >= 0; i--) {
                total_seek_time += abs(sorted_queue[i] - current_position);
                current_position = sorted_queue[i];
            }
        }
    } else {
        // First service all requests below current position
        for (int i = current_index - 1; i >= 0; i--) {
            total_seek_time += abs(sorted_queue[i] - current_position);
            current_position = sorted_queue[i];
        }
        
        // If we reach the beginning, change direction and service remaining requests
        if (current_index < scheduler->queue_size) {
            // Go to the beginning of the disk
            total_seek_time += abs(current_position - 0);
            current_position = 0;
            
            // Then move up, servicing the remaining requests
            for (int i = current_index; i < scheduler->queue_size; i++) {
                total_seek_time += abs(sorted_queue[i] - current_position);
                current_position = sorted_queue[i];
            }
        }
    }
    
    scheduler->current_position = current_position;
    return total_seek_time;
}
```

### SCAN Example Execution
```
Initial head position: 50
Direction: Upward (toward higher track numbers)
Disk size: 200 tracks (0-199)
Request Queue: 95, 180, 34, 119, 11, 123, 62, 64

Sequence of head movement:
50 → 62 (seek distance: 12)
62 → 64 (seek distance: 2)
64 → 95 (seek distance: 31)
95 → 119 (seek distance: 24)
119 → 123 (seek distance: 4)
123 → 180 (seek distance: 57)
180 → 199 (seek distance: 19) [End of disk]
199 → 34 (seek distance: 165)
34 → 11 (seek distance: 23)

Total seek distance: 337
Average seek distance: 42.1
```

## C-SCAN (Circular SCAN)

C-SCAN is a modified version of SCAN that only services requests in one direction.

### Key Concepts
- Moves in one direction only, servicing requests
- Upon reaching the end, returns to the beginning without servicing requests
- Provides more uniform waiting time than SCAN

### Advantages and Disadvantages
- **Advantages**: More uniform wait times, better for heavy loads
- **Disadvantages**: Longer average seek times compared to SCAN in some cases

### Example Implementation
```c
// C-SCAN disk scheduling implementation
typedef struct {
    int request_queue[MAX_REQUESTS];
    int queue_size;
    int current_position;
} CScanScheduler;

void initialize(CScanScheduler *scheduler, int start_position) {
    scheduler->queue_size = 0;
    scheduler->current_position = start_position;
}

void addRequest(CScanScheduler *scheduler, int track) {
    if (scheduler->queue_size < MAX_REQUESTS) {
        scheduler->request_queue[scheduler->queue_size++] = track;
    }
}

int calculateSeekTime(CScanScheduler *scheduler, int disk_size) {
    int total_seek_time = 0;
    int current_position = scheduler->current_position;
    
    // Sort requests for easier processing
    int sorted_queue[MAX_REQUESTS];
    for (int i = 0; i < scheduler->queue_size; i++) {
        sorted_queue[i] = scheduler->request_queue[i];
    }
    qsort(sorted_queue, scheduler->queue_size, sizeof(int), compare);
    
    // Find position where current head is relative to requests
    int current_index = 0;
    while (current_index < scheduler->queue_size && 
           sorted_queue[current_index] < current_position) {
        current_index++;
    }
    
    // First service all requests above current position
    for (int i = current_index; i < scheduler->queue_size; i++) {
        total_seek_time += abs(sorted_queue[i] - current_position);
        current_position = sorted_queue[i];
    }
    
    // If there are requests below the initial position
    if (current_index > 0) {
        // Go to the end of the disk
        total_seek_time += abs(disk_size - 1 - current_position);
        
        // Jump to the beginning (not counted in seek distance)
        current_position = 0;
        
        // Service requests from beginning up to the initial position
        for (int i = 0; i < current_index; i++) {
            total_seek_time += abs(sorted_queue[i] - current_position);
            current_position = sorted_queue[i];
        }
    }
    
    scheduler->current_position = current_position;
    return total_seek_time;
}
```

### C-SCAN Example Execution
```
Initial head position: 50
Disk size: 200 tracks (0-199)
Request Queue: 95, 180, 34, 119, 11, 123, 62, 64

Sequence of head movement:
50 → 62 (seek distance: 12)
62 → 64 (seek distance: 2)
64 → 95 (seek distance: 31)
95 → 119 (seek distance: 24)
119 → 123 (seek distance: 4)
123 → 180 (seek distance: 57)
180 → 199 (seek distance: 19) [End of disk]
Jump to beginning of disk (not counted in seek distance)
0 → 11 (seek distance: 11)
11 → 34 (seek distance: 23)

Total seek distance: 183
Average seek distance: 22.9
```

## Performance Comparison

Different disk scheduling algorithms perform better under different scenarios:

| Algorithm | Advantages | Disadvantages | Best Use Case |
|-----------|------------|--------------|---------------|
| FCFS | Simple, fair | Inefficient, high seek times | Light disk loads, fairness priority |
| SCAN | Efficient, lower average seek time | May favor middle tracks | Heavy disk loads, varied request distribution |
| C-SCAN | Uniform wait times, lower variance | Higher overhead | Heavy disk loads, uniform service requirements |

### Performance Factors
- **Request Distribution**: Scattered vs. localized requests
- **Disk Load**: Heavy vs. light traffic
- **Request Pattern**: Sequential vs. random access
- **Fairness Requirements**: Equal service vs. throughput optimization

## Exercises

1. Implement the SCAN algorithm and compare its performance with FCFS
2. Simulate C-SCAN with various initial head positions and analyze the results
3. Develop a disk scheduling algorithm that combines features of multiple algorithms
4. Analyze the impact of disk size on the performance of different scheduling algorithms
5. Design a real-time disk scheduling algorithm that prioritizes certain requests