# Page Replacement Algorithms

This section covers various page replacement algorithms used in virtual memory management systems:

## Contents
1. [Page Replacement Fundamentals](#page-replacement-fundamentals)
2. [FIFO (First-In-First-Out)](#fifo-first-in-first-out)
3. [LRU (Least Recently Used)](#lru-least-recently-used)
4. [CLOCK (Second-Chance Algorithm)](#clock-second-chance-algorithm)
5. [LFU (Least Frequently Used)](#lfu-least-frequently-used)
6. [MFU (Most Frequently Used)](#mfu-most-frequently-used)
7. [Performance Comparison](#performance-comparison)

## Page Replacement Fundamentals

When a page fault occurs and all frames are currently in use, the operating system must select a page to be replaced.

### Key Concepts
- **Page Fault**: Occurs when a process references a page not in memory
- **Page Frames**: Physical memory divided into fixed-size blocks
- **Replacement Policy**: Algorithm to decide which page to remove
- **Belady's Anomaly**: Phenomenon where increasing the number of frames can increase page faults
- **Thrashing**: Excessive paging causing severe performance degradation

## FIFO (First-In-First-Out)

The simplest page replacement algorithm that replaces the oldest page in memory.

### Key Concepts
- Maintains a queue of pages
- New pages added to the back of the queue
- Replacement removes page from the front of the queue
- Suffers from Belady's anomaly

### Advantages and Disadvantages
- **Advantages**: Simple implementation, low overhead
- **Disadvantages**: Does not consider page usage patterns, often performs poorly

### Example Implementation
```c
// FIFO implementation using a queue
typedef struct {
    int page_frames[MAX_FRAMES];
    int front;
    int count;
} FIFOQueue;

void initialize(FIFOQueue *queue) {
    queue->front = 0;
    queue->count = 0;
}

int replacePage(FIFOQueue *queue, int new_page) {
    int victim_page;
    
    if (queue->count < MAX_FRAMES) {
        // Add page to an empty frame
        queue->page_frames[(queue->front + queue->count) % MAX_FRAMES] = new_page;
        queue->count++;
        return -1;  // No page replaced
    } else {
        // Replace the oldest page (FIFO)
        victim_page = queue->page_frames[queue->front];
        queue->page_frames[queue->front] = new_page;
        queue->front = (queue->front + 1) % MAX_FRAMES;
        return victim_page;
    }
}
```

### FIFO Example Execution
```
Reference String: 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
Frame Size: 3

Step 1: 1 → [1, -, -]       (Page Fault)
Step 2: 2 → [1, 2, -]       (Page Fault)
Step 3: 3 → [1, 2, 3]       (Page Fault)
Step 4: 4 → [4, 2, 3]       (Page Fault, replaced 1)
Step 5: 1 → [4, 1, 3]       (Page Fault, replaced 2)
Step 6: 2 → [4, 1, 2]       (Page Fault, replaced 3)
Step 7: 5 → [5, 1, 2]       (Page Fault, replaced 4)
Step 8: 1 → [5, 1, 2]       (Hit)
Step 9: 2 → [5, 1, 2]       (Hit)
Step 10: 3 → [3, 1, 2]      (Page Fault, replaced 5)
Step 11: 4 → [3, 4, 2]      (Page Fault, replaced 1)
Step 12: 5 → [3, 4, 5]      (Page Fault, replaced 2)

Total Page Faults: 10
```

## LRU (Least Recently Used)

Replaces the page that has not been used for the longest period of time.

### Key Concepts
- Based on the principle of locality
- Keeps track of when each page was last accessed
- Replaces the page with the oldest access time
- Generally performs well but can be costly to implement

### Advantages and Disadvantages
- **Advantages**: Good performance, takes advantage of temporal locality
- **Disadvantages**: Requires hardware support for efficient implementation

### Example Implementation
```c
// LRU implementation using a counter
typedef struct {
    int page_number;
    unsigned long last_used;
} PageFrame;

int findLRUPage(PageFrame frames[], int n_frames) {
    int lru_index = 0;
    unsigned long oldest_time = frames[0].last_used;
    
    for (int i = 1; i < n_frames; i++) {
        if (frames[i].last_used < oldest_time) {
            oldest_time = frames[i].last_used;
            lru_index = i;
        }
    }
    
    return lru_index;
}

bool accessPage(PageFrame frames[], int n_frames, int page, unsigned long current_time) {
    // Check if page is already in a frame
    for (int i = 0; i < n_frames; i++) {
        if (frames[i].page_number == page) {
            frames[i].last_used = current_time;  // Update access time
            return true;  // Page hit
        }
    }
    
    // Page fault - find LRU page to replace
    int lru_index = findLRUPage(frames, n_frames);
    frames[lru_index].page_number = page;
    frames[lru_index].last_used = current_time;
    return false;  // Page fault
}
```

### LRU Example Execution
```
Reference String: 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
Frame Size: 3

Step 1: 1 → [1, -, -]       (Page Fault)
Step 2: 2 → [1, 2, -]       (Page Fault)
Step 3: 3 → [1, 2, 3]       (Page Fault)
Step 4: 4 → [4, 2, 3]       (Page Fault, replaced 1)
Step 5: 1 → [4, 2, 1]       (Page Fault, replaced 3)
Step 6: 2 → [4, 2, 1]       (Hit)
Step 7: 5 → [5, 2, 1]       (Page Fault, replaced 4)
Step 8: 1 → [5, 2, 1]       (Hit)
Step 9: 2 → [5, 2, 1]       (Hit)
Step 10: 3 → [5, 3, 1]      (Page Fault, replaced 2)
Step 11: 4 → [4, 3, 1]      (Page Fault, replaced 5)
Step 12: 5 → [4, 3, 5]      (Page Fault, replaced 1)

Total Page Faults: 8
```

## CLOCK (Second-Chance Algorithm)

A more efficient approximation of LRU that uses a reference bit.

### Key Concepts
- Uses a circular list to represent page frames
- Each page has a reference bit (0 or 1)
- When a page is accessed, its reference bit is set to 1
- Pointer moves around the circular list looking for pages with reference bit 0
- If reference bit is 1, it's set to 0 and the pointer moves to the next page

### Advantages and Disadvantages
- **Advantages**: Approximates LRU with lower overhead
- **Disadvantages**: Not as efficient as true LRU

### Example Implementation
```c
typedef struct {
    int page_number;
    bool reference_bit;
} ClockFrame;

int replacePage(ClockFrame frames[], int n_frames, int *clock_pointer, int new_page) {
    // Check if page is already in memory
    for (int i = 0; i < n_frames; i++) {
        if (frames[i].page_number == new_page) {
            frames[i].reference_bit = true;  // Mark as referenced
            return -1;  // No page fault
        }
    }
    
    // Page fault - find a page to replace
    while (true) {
        if (frames[*clock_pointer].page_number == -1) {
            // Empty frame found
            frames[*clock_pointer].page_number = new_page;
            frames[*clock_pointer].reference_bit = true;
            *clock_pointer = (*clock_pointer + 1) % n_frames;
            return -1;
        } else if (frames[*clock_pointer].reference_bit == false) {
            // Found a non-referenced page to replace
            int victim = frames[*clock_pointer].page_number;
            frames[*clock_pointer].page_number = new_page;
            frames[*clock_pointer].reference_bit = true;
            *clock_pointer = (*clock_pointer + 1) % n_frames;
            return victim;
        } else {
            // Give a second chance
            frames[*clock_pointer].reference_bit = false;
            *clock_pointer = (*clock_pointer + 1) % n_frames;
        }
    }
}
```

### CLOCK Example Execution
```
Reference String: 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
Frame Size: 3

Initial state: All frames empty, pointer at position 0
Initially all reference bits are 0

Step 1: 1 → [1(1), -, -]          (Page Fault)
Step 2: 2 → [1(1), 2(1), -]       (Page Fault)
Step 3: 3 → [1(1), 2(1), 3(1)]    (Page Fault)
Step 4: 4 → [4(1), 2(1), 3(1)]    (Page Fault, replaced 1)
Step 5: 1 → [4(1), 2(1), 1(1)]    (Page Fault, replaced 3)
Step 6: 2 → [4(1), 2(1), 1(1)]    (Hit, set bit to 1)
Step 7: 5 → [5(1), 2(1), 1(1)]    (Page Fault, replaced 4)
Step 8: 1 → [5(1), 2(1), 1(1)]    (Hit, set bit to 1)
Step 9: 2 → [5(1), 2(1), 1(1)]    (Hit, set bit to 1)
Step 10: 3 → [3(1), 2(1), 1(1)]   (Page Fault, replaced 5)
Step 11: 4 → [3(1), 4(1), 1(1)]   (Page Fault, replaced 2)
Step 12: 5 → [3(1), 4(1), 5(1)]   (Page Fault, replaced 1)

Total Page Faults: 9
```

## LFU (Least Frequently Used)

Replaces the page with the smallest number of accesses.

### Key Concepts
- Tracks frequency of access for each page
- Replaces page with lowest access count
- Considers long-term importance of pages

### Advantages and Disadvantages
- **Advantages**: Good for workloads with clear access patterns
- **Disadvantages**: Does not consider recency, can keep old but frequently used pages

### Example Implementation
```c
typedef struct {
    int page_number;
    int frequency;
    unsigned long last_used;  // For breaking ties
} LFUFrame;

int findLFUPage(LFUFrame frames[], int n_frames) {
    int lfu_index = 0;
    int min_freq = frames[0].frequency;
    unsigned long oldest_time = frames[0].last_used;
    
    for (int i = 1; i < n_frames; i++) {
        if (frames[i].frequency < min_freq || 
            (frames[i].frequency == min_freq && frames[i].last_used < oldest_time)) {
            min_freq = frames[i].frequency;
            oldest_time = frames[i].last_used;
            lfu_index = i;
        }
    }
    
    return lfu_index;
}

bool accessPage(LFUFrame frames[], int n_frames, int page, unsigned long current_time) {
    // Check if page is already in a frame
    for (int i = 0; i < n_frames; i++) {
        if (frames[i].page_number == page) {
            frames[i].frequency++;
            frames[i].last_used = current_time;
            return true;  // Page hit
        }
    }
    
    // Page fault - find LFU page to replace
    int lfu_index = findLFUPage(frames, n_frames);
    frames[lfu_index].page_number = page;
    frames[lfu_index].frequency = 1;
    frames[lfu_index].last_used = current_time;
    return false;  // Page fault
}
```

### LFU Example Execution
```
Reference String: 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
Frame Size: 3

Step 1: 1 → [1(1), -, -]       (Page Fault)
Step 2: 2 → [1(1), 2(1), -]    (Page Fault)
Step 3: 3 → [1(1), 2(1), 3(1)] (Page Fault)
Step 4: 4 → [1(1), 2(1), 4(1)] (Page Fault, replaced 3)
Step 5: 1 → [1(2), 2(1), 4(1)] (Hit)
Step 6: 2 → [1(2), 2(2), 4(1)] (Hit)
Step 7: 5 → [1(2), 2(2), 5(1)] (Page Fault, replaced 4)
Step 8: 1 → [1(3), 2(2), 5(1)] (Hit)
Step 9: 2 → [1(3), 2(3), 5(1)] (Hit)
Step 10: 3 → [1(3), 2(3), 3(1)] (Page Fault, replaced 5)
Step 11: 4 → [1(3), 2(3), 4(1)] (Page Fault, replaced 3)
Step 12: 5 → [5(1), 2(3), 4(1)] (Page Fault, replaced 1)

Total Page Faults: 8
```

## MFU (Most Frequently Used)

Replaces the page with the highest number of accesses.

### Key Concepts
- Opposite of LFU
- Based on the assumption that pages with many accesses are likely to be done with
- Not commonly used in practice

### Advantages and Disadvantages
- **Advantages**: Can work well for certain access patterns
- **Disadvantages**: Generally performs worse than other algorithms

### Example Implementation
```c
typedef struct {
    int page_number;
    int frequency;
    unsigned long last_used;  // For breaking ties
} MFUFrame;

int findMFUPage(MFUFrame frames[], int n_frames) {
    int mfu_index = 0;
    int max_freq = frames[0].frequency;
    unsigned long oldest_time = frames[0].last_used;
    
    for (int i = 1; i < n_frames; i++) {
        if (frames[i].frequency > max_freq || 
            (frames[i].frequency == max_freq && frames[i].last_used < oldest_time)) {
            max_freq = frames[i].frequency;
            oldest_time = frames[i].last_used;
            mfu_index = i;
        }
    }
    
    return mfu_index;
}

bool accessPage(MFUFrame frames[], int n_frames, int page, unsigned long current_time) {
    // Check if page is already in a frame
    for (int i = 0; i < n_frames; i++) {
        if (frames[i].page_number == page) {
            frames[i].frequency++;
            frames[i].last_used = current_time;
            return true;  // Page hit
        }
    }
    
    // Page fault - find MFU page to replace
    int mfu_index = findMFUPage(frames, n_frames);
    frames[mfu_index].page_number = page;
    frames[mfu_index].frequency = 1;
    frames[mfu_index].last_used = current_time;
    return false;  // Page fault
}
```

### MFU Example Execution
```
Reference String: 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
Frame Size: 3

Step 1: 1 → [1(1), -, -]        (Page Fault)
Step 2: 2 → [1(1), 2(1), -]     (Page Fault)
Step 3: 3 → [1(1), 2(1), 3(1)]  (Page Fault)
Step 4: 4 → [4(1), 2(1), 3(1)]  (Page Fault, replaced 1)
Step 5: 1 → [4(1), 1(1), 3(1)]  (Page Fault, replaced 2)
Step 6: 2 → [4(1), 1(1), 2(1)]  (Page Fault, replaced 3)
Step 7: 5 → [4(1), 5(1), 2(1)]  (Page Fault, replaced 1)
Step 8: 1 → [4(1), 5(1), 1(1)]  (Page Fault, replaced 2)
Step 9: 2 → [4(1), 2(1), 1(1)]  (Page Fault, replaced 5)
Step 10: 3 → [3(1), 2(1), 1(1)] (Page Fault, replaced 4)
Step 11: 4 → [3(1), 4(1), 1(1)] (Page Fault, replaced 2)
Step 12: 5 → [3(1), 4(1), 5(1)] (Page Fault, replaced 1)

Total Page Faults: 12
```

## Performance Comparison

Different algorithms perform better under different workloads:

| Algorithm | Advantages | Disadvantages | Performance |
|-----------|------------|--------------|-------------|
| FIFO | Simple implementation | Belady's anomaly, poor performance | Poor |
| LRU | Good performance, respects locality | Complex implementation | Good |
| CLOCK | Reasonable performance, lower overhead | Not optimal | Good |
| LFU | Works well for stable access patterns | Does not consider recency | Fair |
| MFU | Works for unusual access patterns | Generally poor performance | Poor |

### Memory Access Patterns
- **Sequential Access**: FIFO performs reasonably
- **Locality of Reference**: LRU performs well
- **Cyclic Access**: FIFO may perform poorly, LRU better
- **Random Access**: All algorithms perform similarly

## Exercises

1. Implement the CLOCK algorithm and compare its page fault rate with FIFO and LRU
2. Design a hybrid algorithm that combines LFU and LRU approaches
3. Demonstrate Belady's anomaly using the FIFO algorithm
4. Simulate page replacement algorithms on a real-world memory trace
5. Implement a predictive page replacement algorithm that tries to anticipate future accesses