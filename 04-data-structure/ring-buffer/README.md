# Pure C Ring Buffer & Function Pointer Callback

## 1. Overview
This project implements a pure C Circular Ring Buffer data structure integrated with a Function Pointer Callback mechanism. 

It simulates hardware interrupt driven event handling (such as STM32 UART RX Interrupt Service Routines), enabling clean architectural decoupling between low-level buffer management and upper-layer application processing.

## 2. Learning Objectives
- Design a thread-safe / ISR-ready Ring Buffer data structure in pure C.
- Master modular arithmetic `(index + 1) % SIZE` for boundary wrap-around protection.
- Implement Function Pointer callbacks to achieve behavioral decoupling.
- Optimize memory usage by passing struct instances by reference (`ring_buffer_t*`).

## 3. Key Concepts
- **Circular Buffer**: Fixed-size memory storage operating as a FIFO queue using head and tail pointers.
- **Function Pointer Callback**: Registering upper-layer event handlers via `typedef void (*ring_buffer_callback_t)(int)`.
- **Decoupling**: The buffer module manages memory and state without needing knowledge of upper-layer business logic.
- **Modulo Arithmetic**: Safe boundary navigation preventing array index out-of-bounds errors.

## 4. Implementation
- `ring_buffer.h`: Struct definitions, function pointer callback types, and module API prototypes.
- `ring_buffer.c`: Core ring buffer logic (`init`, `push`, `pop`, `isFull`, `isEmpty`) and callback dispatch loop (`ring_buffer_process`).
- `main.c`: Unit test simulating UART data arrival and processing via callback handlers.

## 5. Project Structure
```text
.
├── ring_buffer.h
├── ring_buffer.c
├── main.c
├── Makefile
└── README.md

6. Build & Run

Bash
# Compile the module
make

# Run unit tests
./ring_buffer_test

# Clean build artifacts
make clean

7. Verification
Verified full cycle execution:

Pushed element sequence (10, 20, 30, 40).

Processed elements via function pointer callback (on_data_received).

Verified buffer state returned to empty (isEmpty == true).

8. What I Learned
How callback functions decouple drivers from application logic in firmware development.

Why function pointer signatures must strictly match arguments to avoid runtime stack corruption.

Common pitfalls in pointer manipulation (e.g., passing double pointers ** instead of single pointers *).