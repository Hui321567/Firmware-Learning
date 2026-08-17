# Linux Process Management & Inter-Process Communication (IPC)

## 1. Overview
This project explores Linux process lifecycles, process creation, execution overriding, zombie process handling, and inter-process communication (IPC) via anonymous pipes.

## 2. Learning Objectives
- Master process creation and lifecycle management (`fork`, `execvp`, `waitpid`).
- Prevent and clean up zombie processes.
- Implement anonymous pipes for parent-child IPC.
- Understand Linux file descriptor duplication in pipes.

## 3. Key Concepts
- **Process Creation**: `fork()` duplicating the calling process.
- **Program Execution**: Replacing process image via `execvp()`.
- **Process Synchronization**: Preventing resource leaks and zombie states using `waitpid()`.
- **Inter-Process Communication**: Unidirectional data streaming via `pipe()`.

## 4. Implementation
- `fork_demo.c`: Demonstrates `fork()`, process isolation, and child waiting.
- `pipe_demo.c`: Demonstrates anonymous pipe communication between parent and child processes.

## 5. Project Structure
```text
.
├── fork_demo.c
├── pipe_demo.c
├── Makefile
└── README.md

6. Build & Run

Bash
# Compile binaries
make

# Execute fork demonstration
./fork_demo

# Execute pipe IPC demonstration
./pipe_demo

# Clean build artifacts
make clean

7. Verification
Verified process PIDs, exit statuses, and piped message delivery via standard terminal execution.

8. What I Learned
Memory isolation between parent and child processes after fork().

How anonymous pipes manage read/write file descriptors across processes.