# POSIX Thread (Pthread) & Mutex Synchronization

This module demonstrates multi-threading and task synchronization in Linux using POSIX Threads (`pthread_t`) and Mutex locks (`pthread_mutex_t`).

## Key Concepts

1. **Multi-Threading**:
    - Creating the concurrent threads within the same process memory space using `pthread_create()`.
    - Synchronizing thread termination and collecting return status using `pthread_join()`.
2. **Data Race & Critical Section**:
    - Multiple threads accessing shared memory (e.g., `counter`) simultaneously without protection causes Data Race, leading to inconsistent state.
3. **Mutex Protection (`pthread_mutex_t`)**:
    - Using `pthread_mutex_lock()` and `pthread_mutex_unlock()` to enforce mutual exclusion on critical sections, ensuring thread safety. 


## Code Structure

- `thread_demo.c`: Spawns 2 concurrent worker threads that safely increment a shared counter using mutex protection.
- `Makefile`: Build setup with the `-pthread` compilation flag.

## Build & Run

```bash
# Build the thread_demo
make

# Run the demo
./thread_demo

# Clean the build artifacts
make clean
