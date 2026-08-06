# Linux File I/O - my_cat

A simple C implementation of the `cat` command to demonstrate file operations in Linux.

## Features
- Reads and prints file contents line by line.
- Proper error handling for missing arguments or missing files.
- Built with standard GCC flags (`-Wall -Wextra -g`).
- Verified with Valgrind for zero memory leaks.

## How to Build and Run
```bash
make
./my_cat <filename>
valgrind --leak-check=full ./my_cat <filename>