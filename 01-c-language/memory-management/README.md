# C Memory Management & Pointer Operations

## 1. Overview
This project demonstrates key C memory concepts including pointer arithmetic, dynamic memory allocation, structure padding, and memory alignment principles critical for firmware engineering.

## 2. Learning Objectives
- Understand pointer arithmetic and memory addressing.
- Analyze struct padding and alignment behaviors of compilers.
- Master dynamic memory allocation (`malloc`, `free`) and avoid memory leaks.
- Practice memory debugging tools like Valgrind.

## 3. Key Concepts
- **Pointer Arithmetic**: Navigating raw byte streams using typed pointers.
- **Struct Padding**: Data alignment rules imposed by CPU architectures (e.g., 32-bit boundary).
- **Dynamic Allocation**: Heap management with `malloc()`, `calloc()`, and `free()`.
- **Memory Safety**: Preventing dangling pointers, buffer overflows, and memory leaks.

## 4. Implementation
Demonstrated across dedicated test source files:
* `pointer_arithmetic.c`: Pointer increments and memory distance calculations.
* `padding_test.c`: Observing compiler memory padding effects on structs.
* `dynamic_memory.c`: Safe allocation, initialization, and deallocation routines.

## 5. Project Structure
```text
.
├── dynamic_memory.c
├── padding_test.c
├── pointer_arithmetic.c
├── Makefile
└── README.md
```

## 6. Build & Run

```Bash
# Compile all binaries
make

# Run pointer arithmetic test
./pointer_arithmetic

# Run struct padding test
./padding_test

# Run dynamic memory test
./dynamic_memory

# Clean build artifacts
make clean
```
## 7. Verification / Debugging
Checked for zero memory leaks using Valgrind:

Bash
valgrind --leak-check=full ./dynamic_memory

## 8. What I Learned
How CPU alignment requirements introduce compiler padding bytes inside structs.

How pointer type casting affects byte offsets during memory traversal.