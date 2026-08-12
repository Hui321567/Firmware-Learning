# Linux File I/O & System Calls

This module demonstrates low-level Linux file operations using POSIX system call interfaces instead of standard C library `stdio` abstractions. 

## Key Features
1. **POSIX System Calls**:
    - Uses `open()`, `read()`, `write()`, and `close()` directly to interact with Linux Kernel File Descripitors (FDs).
2. **Zero-Buffer Abstraction**:
    - Bypasses Libc stream buffering for direct user-to-kernel memory transfer via user-mangaed byte buffers.
3. **Standard File Descripitors**:
    - Direct output stream handling using `STDOUT_FILENO`(1) and `STDERR_FILENO`(2).

## Implementation (`io_test`)

Implements a custom, lightweight version of the Linux `io_test` command line utility using direct file descripitor manipulation. 

## How to Build & Run
```bash

# Build the utility
make

# Test reading a text file
./io_test text.txt

# Clean build artifacts
make clean 