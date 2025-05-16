# Processes & Inter-Process Communication

## Contents
1. [Process Fundamentals](#process-fundamentals)
2. [Process Creation](#process-creation)
3. [IPC: Shared Memory](#ipc-shared-memory)
4. [IPC: Message Queues](#ipc-message-queues) 
5. [IPC: Pipes](#ipc-pipes)
6. [IPC: Sockets](#ipc-sockets)
7. [IPC: Remote Procedure Calls](#ipc-remote-procedure-calls)

## Process Fundamentals

A process is an instance of a program in execution. It's a unit of resource allocation and protection.

### Key Concepts
- **Process Image**: Program code, data, stack, heap, and other resources
- **Process Control Block (PCB)**: Data structure containing process information
- **Context Switch**: Saving/restoring state when switching between processes
- **Process States**: New, Ready, Running, Waiting, Terminated
- **Process ID (PID)**: Unique identifier for each process

### Process vs. Thread
- Processes have separate memory spaces
- Threads share the memory space of their parent process
- Process creation is more resource-intensive than thread creation
- IPC is required for processes to communicate, threads can share data directly

## Process Creation

In POSIX systems, processes are created using `fork()` and can be transformed using `exec()` family functions.

### Basic Fork Example
```c
#include <unistd.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        // Error handling
        fprintf(stderr, "Fork failed\n");
    } else if (pid == 0) {
        // Child process code
        printf("Child process: PID = %d\n", getpid());
    } else {
        // Parent process code
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);
    }
    
    return 0;
}
```

### Exec Example
```c
#include <unistd.h>
#include <stdio.h>

int main() {
    // Replace current process with ls command
    execl("/bin/ls", "ls", "-l", NULL);
    
    // If exec returns, it failed
    perror("exec failed");
    return 1;
}
```

## IPC: Shared Memory

POSIX shared memory allows multiple processes to access a common memory region.

### Basic Shared Memory Example
```c
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Create and open shared memory object
    int fd = shm_open("/myshm", O_CREAT | O_RDWR, 0666);
    
    // Set size
    ftruncate(fd, 4096);
    
    // Map into address space
    void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    // Write to shared memory
    strcpy((char *)addr, "Hello via shared memory");
    
    // Clean up
    munmap(addr, 4096);
    close(fd);
    // shm_unlink("/myshm"); // Uncomment to remove the shared memory object
    
    return 0;
}
```

## IPC: Message Queues

POSIX message queues provide a way for processes to exchange data in the form of messages.

### Message Queue Example
```c
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

int main() {
    // Create/open message queue
    mqd_t mq = mq_open("/myqueue", O_CREAT | O_RDWR, 0666, NULL);
    
    // Send message
    const char *msg = "Hello via message queue";
    mq_send(mq, msg, strlen(msg), 0);
    
    // Clean up
    mq_close(mq);
    // mq_unlink("/myqueue"); // Uncomment to remove the queue
    
    return 0;
}
```

## IPC: Pipes

Pipes provide a simple unidirectional communication channel between related processes.

### Unnamed Pipe Example
```c
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];
    pipe(pipefd); // Create pipe
    
    if (fork() == 0) { // Child
        close(pipefd[1]); // Close unused write end
        
        char buf[100];
        read(pipefd[0], buf, sizeof(buf));
        printf("Received: %s\n", buf);
        
        close(pipefd[0]);
    } else { // Parent
        close(pipefd[0]); // Close unused read end
        
        const char *msg = "Hello via pipe";
        write(pipefd[1], msg, strlen(msg) + 1);
        
        close(pipefd[1]);
    }
    
    return 0;
}
```

### Named Pipe (FIFO) Example
```c
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    // Create FIFO
    mkfifo("/tmp/myfifo", 0666);
    
    // Open for writing (in a real example, reading would be in another process)
    int fd = open("/tmp/myfifo", O_WRONLY);
    
    // Write data
    const char *msg = "Hello via FIFO";
    write(fd, msg, strlen(msg) + 1);
    
    // Clean up
    close(fd);
    unlink("/tmp/myfifo");
    
    return 0;
}
```

## IPC: Sockets

Sockets provide a communication mechanism between processes running on the same or different machines.

### Key Concepts
- Support both local (Unix domain) and network (Internet domain) communication
- Bidirectional communication
- Can use TCP (connection-oriented) or UDP (connectionless)
- POSIX socket API includes: `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `send()`, `recv()`

Unix domain sockets are used for local IPC and are more efficient than network sockets when communicating between processes on the same machine. They use file system pathnames for addressing.

Network sockets allow communication across different machines using IP addresses and ports. They're the foundation of network programming and distributed systems.

## IPC: Remote Procedure Calls

Remote Procedure Calls (RPC) allow a program to execute procedures on another computer or in another process.

### Key Concepts
- Makes remote procedure calls appear like local function calls
- Abstracts away network communication details
- Client stub marshals parameters and handles communication
- Server stub receives requests and invokes the actual procedure
- Results are sent back to the client

Modern RPC frameworks include gRPC (by Google), which uses Protocol Buffers for efficient serialization and HTTP/2 for communication. Traditional POSIX systems might use Sun RPC/ONC RPC.

RPCs are fundamental to distributed systems, microservices, and client-server applications.
