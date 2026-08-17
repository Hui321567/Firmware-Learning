# Linux POSIX Threads & Producer-Consumer Synchronization

## 1. Overview
This project demonstrates POSIX multi-threading (`pthreads`) and thread synchronization primitives in Linux, featuring a Producer-Consumer architecture with a shared Ring Buffer.

## 2. Learning Objectives
- Master POSIX thread creation and termination management (`pthread_create`, `pthread_join`).
- Prevent race conditions using Mutex locks (`pthread_mutex_t`).
- Implement event-driven thread signaling with Condition Variables (`pthread_cond_t`).
- Design a thread-safe Producer-Consumer pattern.

## 3. Key Concepts
- **Multithreading**: Concurrent execution paths sharing the same process memory space.
- **Mutex Lock**: Mutual exclusion protecting critical sections.
- **Condition Variables**: Thread sleeping and signaling mechanism to prevent CPU polling.
- **Producer-Consumer Pattern**: Asynchronous data exchange via shared circular storage.

## 4. Implementation
- `thread_demo.c`: Basic thread creation, arguments passing, and thread joining.
- `cond_demo.c`: Producer-Consumer pattern using Mutex and Condition Variables.

## 5. Project Structure
```text
.
├── cond_demo.c
├── thread_demo.c
├── Makefile
└── README.md

6. Build & Run

Bash
# Compile
make

# Run thread creation demo
./thread_demo

# Run Producer-Consumer synchronization test
./cond_demo

# Clean build artifacts
make clean

7. Verification
Verified that race conditions are prevented, threads sleep properly without maxing out CPU utilization, and bounded buffers do not overflow or underflow.

8. What I Learned
Why while loops are mandatory around pthread_cond_wait() to handle spurious wakeups.

How thread synchronization patterns directly mirror hardware interrupt/main-loop event handling in RTOS.