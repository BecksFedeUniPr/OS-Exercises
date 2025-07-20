#include <stdio.h>
#include "../src/algorithms/fcfs.h"
#include "../src/utils/metrics.h"

int main() {
    printf("=== Testing FCFS Algorithm ===\n");
    
    // Test case with known results
    Process test_processes[] = {
        {1, 0, 10, 0},
        {2, 1, 4, 0},
        {3, 2, 6, 0}
    };
    int num_processes = 3;
    
    // Initialize processes
    for (int i = 0; i < num_processes; i++) {
        init_process(&test_processes[i], test_processes[i].id, 
                    test_processes[i].arrival_time,
                    test_processes[i].burst_time,
                    test_processes[i].priority);
    }
    
    // Expected results:
    // P1: TAT=10, WT=0, RT=0
    // P2: TAT=13, WT=9, RT=9
    // P3: TAT=18, WT=12, RT=12
    
    // Run algorithm
    schedule_fcfs(test_processes, num_processes);
    
    // Verify results
    int passed = 1;
    if (test_processes[0].turnaround_time != 10 || test_processes[0].waiting_time != 0) {
        printf("FAIL: P1 metrics incorrect\n");
        passed = 0;
    }
    if (test_processes[1].turnaround_time != 13 || test_processes[1].waiting_time != 9) {
        printf("FAIL: P2 metrics incorrect\n");
        passed = 0;
    }
    if (test_processes[2].turnaround_time != 18 || test_processes[2].waiting_time != 12) {
        printf("FAIL: P3 metrics incorrect\n");
        passed = 0;
    }
    
    if (passed) {
        printf("All tests PASSED!\n");
    }
    
    return passed ? 0 : 1;
}