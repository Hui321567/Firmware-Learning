# Linux Process Managment & Inter-Process Communication (IPC)

This module explores core Linux system programming concepts, including process lifecycle management, image replacement, resource cleanup, and Inter-Process Coummnication (IPC) via anonymous pipes.

## Topics Covered

1. **Process Lifecycle (`fork_demo.c`)**
    - **`fork()`**: Process duplication and Copy-on-Write (COW) concept.
    - **`execvp()`**: Replaceing child process memory image with system commands (`ls -l`).
    - **`waitpid()`**: Parent waiting for child exit status to avoid zambie processes.

2. **Inter-Process Communication (`pipe_demo.c`)**
    - **`pipe()`**: Creating unidirectional byte-stream channels using File Descriptors (`pipefd[0]` for read, `pipefd[1]` for write).
    - **Safe I/O Handling**: Managing despriptors and appending null terminators (`'\0'`) based on actual bytes read.

## Build & Run

```bash
# Compile all demos
make

# Run process lifecycle demo
./fork_demo

# Run pipe IPC demo
./pipe_demo

# Clean build artifacts
make clean