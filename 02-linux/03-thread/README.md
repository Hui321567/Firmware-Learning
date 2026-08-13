# POSIX Thread (Pthread) & Condtion Variables

This module demonstrates multi-threading, task synchronization, and thread communication in Linux using POSIX Threads (`pthread_t`), Mutex locks (`pthread_mutex_t`),and Condition Variables (`pthread_cond_t`).

## Key Concepts

1. **Multi-Threading(`pthread_create`, `pthread_join`)**:
    - Creating the concurrent threads within the same process memory space using .
2. **Mutex Protection(`pthread_mutex_t`)**:
    - Using `pthread_mutex_lock()` and `pthread_mutex_unlock()` to enforce mutual exclusion on critical sections, ensuring thread safety. 
3. **Condition Variables(`pthread_cond_t`)**:
    - Using `pthread_cond_wait()` and `pthread_cond_signal()` to synchronize producer and consumer operation.


## Code Structure

- `thread_demo.c`: Spawns 2 concurrent worker threads that safely increment a shared counter using mutex protection.
- `cond_demo.c`: Advanced Producer-Consumer pattern using Condition Variables and Ring Buffer
- `Makefile`: Build setup with the `-pthread` compilation flag.

## Build & Run

```bash
# Build the thread_demo and cond_demo
make

# Run the thread_demo
./thread_demo

# Run the cond_demo

# Clean the build artifacts
make clean
