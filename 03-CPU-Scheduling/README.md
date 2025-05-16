# CPU Scheduling Algorithms

This section covers various CPU scheduling algorithms used in operating systems to manage process execution:

## Contents
1. [CPU Scheduling Fundamentals](#cpu-scheduling-fundamentals)
2. [FCFS (First-Come First-Served)](#fcfs-first-come-first-served)
3. [SJF (Shortest Job First)](#sjf-shortest-job-first)
4. [SRTF (Shortest Remaining Time First)](#srtf-shortest-remaining-time-first)
5. [RR (Round Robin)](#rr-round-robin)
6. [Priority Scheduling](#priority-scheduling)
7. [Performance Comparison](#performance-comparison)

## CPU Scheduling Fundamentals

CPU scheduling is the process of determining which process or thread should be allocated CPU time at any given moment.

### Key Concepts
- **CPU Burst**: Period of time during which a process executes on the CPU
- **I/O Burst**: Period of time during which a process performs I/O operations
- **Preemptive Scheduling**: CPU can be taken away from a process
- **Non-preemptive Scheduling**: Process keeps CPU until completion or blocking
- **Scheduling Metrics**: Throughput, turnaround time, waiting time, response time

## FCFS (First-Come First-Served)

The simplest CPU scheduling algorithm where processes are executed in the order they arrive in the ready queue.

### Key Concepts
- Non-preemptive scheduling
- Processes are executed in arrival order
- Simple to implement and understand

### Advantages and Disadvantages
- **Advantages**: Simple implementation, fair to processes in arrival order
- **Disadvantages**: Can lead to "convoy effect" where short processes wait behind long ones

### Example Concept
```c
// Basic FCFS concept
for (each process in order of arrival) {
    execute_process_to_completion();
    calculate_metrics();
}
```

### FCFS Example Execution
```
Process Queue: P1 (arrival: 0, burst: 24), P2 (arrival: 1, burst: 3), P3 (arrival: 2, burst: 3)

Execution Sequence:
0-24: P1 executes
24-27: P2 executes
27-30: P3 executes

Process | Turnaround Time | Waiting Time
--------|----------------|-------------
P1      | 24             | 0
P2      | 26             | 23
P3      | 28             | 25

Average Waiting Time: 16.0
Average Turnaround Time: 26.0
```

## SJF (Shortest Job First)

A scheduling algorithm that selects the process with the smallest execution time to run next.

### Key Concepts
- Can be preemptive or non-preemptive
- Requires knowledge or estimation of execution times
- Minimizes average waiting time

### Advantages and Disadvantages
- **Advantages**: Optimal average waiting time for given processes
- **Disadvantages**: Potential starvation of longer processes, requires prediction of burst times

### Example Concept
```c
// Basic SJF concept
while (processes_remain) {
    find_process_with_shortest_burst_time();
    execute_process_to_completion();
    update_metrics();
}
```

### SJF Example Execution
```
Process Queue: P1 (arrival: 0, burst: 6), P2 (arrival: 1, burst: 8), P3 (arrival: 2, burst: 3), P4 (arrival: 3, burst: 4)

Execution Sequence:
0-6: P1 executes (shortest job at time 0)
6-9: P3 executes (shortest job at time 6)
9-13: P4 executes (shortest job at time 9)
13-21: P2 executes (last remaining job)

Process | Turnaround Time | Waiting Time
--------|----------------|-------------
P1      | 6              | 0
P2      | 20             | 12
P3      | 7              | 4
P4      | 10             | 6

Average Waiting Time: 5.5
Average Turnaround Time: 10.75
```

## SRTF (Shortest Remaining Time First or SRTJ) 

The preemptive version of SJF where the process with the shortest remaining time is selected to run.

### Key Concepts
- Preemptive scheduling
- Process can be interrupted when a new process with shorter remaining time arrives
- Optimal average waiting time

### Advantages and Disadvantages
- **Advantages**: Minimizes average waiting time, responsive to short processes
- **Disadvantages**: Overhead from context switching, starvation of longer processes, requires continuous monitoring

### Example Concept
```c
// Basic SRTF concept
while (processes_remain) {
    find_process_with_shortest_remaining_time();
    execute_process_for_one_unit();
    update_remaining_times();
    check_for_completed_processes();
}
```

### SRTF Example Execution
```
Process Queue: P1 (arrival: 0, burst: 8), P2 (arrival: 1, burst: 4), P3 (arrival: 2, burst: 2), P4 (arrival: 3, burst: 1)

Execution Sequence:
0-1: P1 executes (only process available)
1-2: P2 executes (shorter remaining time than P1)
2-3: P3 executes (shorter remaining time than P1, P2)
3-4: P4 executes (shortest remaining time)
4-5: P3 executes (1 time unit remaining)
5-8: P2 executes (3 time units remaining)
8-16: P1 executes (7 time units remaining)

Process | Turnaround Time | Waiting Time
--------|----------------|-------------
P1      | 16             | 8
P2      | 7              | 3
P3      | 3              | 1
P4      | 1              | 0

Average Waiting Time: 3.0
Average Turnaround Time: 6.75
```

## RR (Round Robin)

A preemptive scheduling algorithm that assigns a fixed time slice to each process in a cyclic order.

### Key Concepts
- Preemptive scheduling
- Uses time quantum (time slice)
- Ready queue is treated as a circular queue

### Advantages and Disadvantages
- **Advantages**: Fair allocation, prevents starvation, good response time
- **Disadvantages**: Higher average waiting time, sensitive to quantum size

### Example Concept
```c
// Basic Round Robin concept
while (processes_remain) {
    for (each process in ready_queue) {
        execute_process_for_time_quantum_or_completion();
        if (process_not_completed)
            add_to_end_of_ready_queue();
    }
}
```

### RR Example Execution
```
Process Queue: P1 (arrival: 0, burst: 10), P2 (arrival: 0, burst: 5), P3 (arrival: 0, burst: 8)
Time Quantum: 2

Execution Sequence:
0-2: P1 executes (2 of 10 units)
2-4: P2 executes (2 of 5 units)
4-6: P3 executes (2 of 8 units)
6-8: P1 executes (4 of 10 units)
8-10: P2 executes (4 of 5 units)
10-12: P3 executes (4 of 8 units)
12-14: P1 executes (6 of 10 units)
14-15: P2 executes (5 of 5 units) - P2 completes
15-17: P3 executes (6 of 8 units)
17-19: P1 executes (8 of 10 units)
19-21: P3 executes (8 of 8 units) - P3 completes
21-23: P1 executes (10 of 10 units) - P1 completes

Process | Turnaround Time | Waiting Time
--------|----------------|-------------
P1      | 23             | 13
P2      | 15             | 10
P3      | 21             | 13

Average Waiting Time: 12.0
Average Turnaround Time: 19.7
```

## Priority Scheduling

A scheduling algorithm that assigns a priority to each process and schedules based on priority.

### Key Concepts
- Can be preemptive or non-preemptive
- Priority can be defined internally (system-defined) or externally (user-defined)
- Higher priority processes are scheduled first

### Advantages and Disadvantages
- **Advantages**: Flexibility in managing system/user objectives
- **Disadvantages**: Starvation of low-priority processes, priority inversion problems

### Example Concept
```c
// Basic Priority Scheduling concept
while (processes_remain) {
    find_highest_priority_ready_process();
    execute_process_to_completion(); // non-preemptive
    update_metrics();
}
```

### Priority Scheduling Example Execution
```
Process Queue: P1 (arrival: 0, burst: 10, priority: 3), P2 (arrival: 0, burst: 5, priority: 1), 
P3 (arrival: 0, burst: 8, priority: 4), P4 (arrival: 0, burst: 2, priority: 2)
Higher number means higher priority

Execution Sequence:
0-8: P3 executes (highest priority 4)
8-18: P1 executes (next highest priority 3)
18-20: P4 executes (next highest priority 2)
20-25: P2 executes (lowest priority 1)

Process | Turnaround Time | Waiting Time
--------|----------------|-------------
P1      | 18             | 8
P2      | 25             | 20
P3      | 8              | 0
P4      | 20             | 18

Average Waiting Time: 11.5
Average Turnaround Time: 17.75
```

## Performance Comparison

Different scheduling algorithms perform better on different metrics:

| Algorithm | Average Waiting Time | Response Time | Overhead | Starvation Risk |
|-----------|---------------------|--------------|----------|-----------------|
| FCFS | Poor | Poor for later processes | Low | No |
| SJF | Optimal (non-preemptive) | Fair | Low | Yes |
| SRTF | Optimal (preemptive) | Good | High | Yes |
| RR | Fair | Excellent | Medium | No |
| Priority | Depends on priority assignment | Depends on priority | Medium | Yes |

### Key Performance Factors
- **Burst time distribution**: Short vs. long processes
- **Arrival patterns**: All at once vs. scattered
- **System type**: Batch, interactive, or real-time
- **Process characteristics**: CPU-bound vs. I/O-bound

## Exercises

1. Implement and compare the waiting times for FCFS, SJF, and RR algorithms using the same set of processes
2. Design a scheduling algorithm that addresses the starvation problem in SJF
3. Analyze how different time quantum values affect the performance of Round Robin scheduling
4. Implement aging in Priority Scheduling to prevent starvation
5. Develop a multi-level feedback queue scheduler with different algorithms at each level

