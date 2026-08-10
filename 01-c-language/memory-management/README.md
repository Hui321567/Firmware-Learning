# C Memory Management & Pointer Operations

This module covers the core C memory concepts,struct alignment,pointer arithmetic, and safe dynamic memory management.

## Topics Covered

1. **Struct Padding & Alignment('padding_test.c')**
    -Memeory alignment rules & padding byte insertion.
    -GCC'__attribute((packed))'usage,hardware register mapping,and alignment fault trade-offs.

2. **Pointer Arithmetic & Type Casting('pointer_arithmetic.c')**
    -Understanding pointer strides based on pointer type sizes ('sizeof(*ptr)').
    -Different between 'a'('int*') and '&a'('int(*)[5]').
    -Pointer arithmetic and explicit casting in C.

3. **Dynamic Memeory & Safety('dynamic_memory.c')**
    -Safe heap allocation using 'malloc' and 'free'.
    -Prevetion of dangling pointers by setting pointers to 'NULL' after freeing.
    -Memory leak inspection using **valgrind**


## Build & Test

```bash
#Buld all targets
make

#Run excutables
./padding_test
./pointer_arithmetic
./dynamic_memory

#Memory leak check
valgrind --leak-check=full ./dynamic_memory

#Clean build artifacts
make clean