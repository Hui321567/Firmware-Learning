# Linux Mini-Shell Implementation

## 1. Overview
This project implements a lightweight custom command-line interface (Shell) in C, demonstrating core Linux system programming concepts including process creation, program execution overriding, and process synchronization.

## 2. Learning Objectives
- Master process lifecycle management using `fork()`, `execvp()`, and `waitpid()`.
- Implement user input parsing and argument tokenization.
- Understand how operating system shells interface between users and system APIs.
- Prevent resource leaks and zombie processes during asynchronous execution.

## 3. Key Concepts
- **Process Creation (`fork`)**: Clones the parent shell process to create a child execution context.
- **Program Execution (`execvp`)**: Replaces the child process image with the target binary (e.g., `ls`, `pwd`).
- **Process Harvesting (`waitpid`)**: Synchronizes the parent process, waiting for child termination to prevent zombie processes.
- **String Tokenization (`strtok`)**: Breaks user command input into an array of argument pointers terminating with `NULL`.

## 4. Implementation
- `main.c`: Implements the Read-Eval-Print Loop (REPL), command parser, built-in commands (`exit`), and process dispatch logic.

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

# Test commands inside myshell
myshell> ls -l
myshell> pwd
myshell> cat Makefile
myshell> exit

# Clean build artifacts
make clean
```

## 7. Verification / Debugging
1. Command Execution: Verified standard Linux commands (ls, pwd, cat) execute and display output correctly.

2. Process Lifecycle: Verified via ps that child processes terminate and are harvested cleanly by waitpid() without leaving zombie processes (Z state).

3. Memory Safety: Built with -Wall -Wextra -std=c11 resulting in zero compiler warnings.

## 8. What I Learned
- Why execvp() requires a NULL-terminated pointer array as its second argument.

- The difference between string character assignment ('\0') and string literals ("\0").

- How shell interfaces bridge command strings to low-level kernel system calls.