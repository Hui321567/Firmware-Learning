# Linux Mini-Shell & Pipeline Implementation

## 1. Overview
This project implements a custom command-line interface (Shell) in C, demonstrating core Linux system programming concepts including process creation, program execution overriding, and inter-process communication (IPC) using anonymous pipes.

## 2. Learning Objectives
- Master process lifecycle management using `fork()`, `execvp()`, and `waitpid()`.
- Implement user input parsing and argument tokenization.
- Understand IPC using `pipe()` and file descriptor redirection via `dup2()`.
- Prevent resource leaks, dangling file descriptors, and zombie processes.

## 3. Key Concepts
- **Process Creation (`fork`)**: Clones the shell process to create child execution contexts.
- **Program Execution (`execvp`)**: Replaces child process images with target binaries (e.g., `ls`, `grep`).
- **Process Synchronization (`waitpid`)**: Synchronizes the parent process, harvesting terminated child processes.
- **Pipeline Communication (`pipe` & `dup2`)**: Redirects `STDOUT` of the left command to the writing end of a pipe and `STDIN` of the right command to the reading end.

## 4. Implementation
- `main.c`: Contains the REPL loop, string tokenization, single-command execution, and two-stage pipeline process management (`execute_pipe`).

## 5. Project Structure
```text
.
├── main.c
├── Makefile
└── README.md
```
## 6. Build & Run
```Bash
# Compile binary
make

# Execute mini-shell
./myshell

# Test pipeline commands inside myshell
myshell> ls -l | grep main
myshell> cat Makefile | grep CC
myshell> exit

# Clean build artifacts
make clean
```
## 7. Verification / Debugging
1. Pipeline Redirection: Verified ls | grep main and cat Makefile | grep CC process inputs/outputs via IPC pipes correctly.

2. Resource & Process Safety: Confirmed file descriptors are explicitly closed in parent/child processes to avoid pipeline hangs, and children are reaped via waitpid().

3. Compiler Diagnostics: Compiled with -Wall -Wextra -std=c11 resulting in zero warnings.

## 8. What I Learned
- How dup2() replaces standard file descriptors (STDIN/STDOUT) with pipe descriptors.

- Why both ends of the pipe must be closed in the parent process to allow child processes to receive EOF.

- How to structure robust error handling and process cleanup when fork() fails.