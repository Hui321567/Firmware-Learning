# Firmware Learning Journey 🚀

A hands-on repository focused on **Embedded Firmware & System Software Engineering**.

This repository documents my systematic progression from low-level C programming and Linux system programming to ARM Cortex-M (STM32) bare-metal driver development and real-time data structures.

---

## 🎯 Learning Focus

* **Core C & Memory**: Bitwise operations, pointer arithmetic, memory alignment, struct padding.
* **Linux System Programming**: File descriptors, system calls, process management, POSIX signals, local IPC.
* **POSIX Threads & Concurrency**: Multi-threading, mutex locks, condition variables, Producer-Consumer pattern.
* **Embedded Data Structures**: Circular Ring Buffer, Queue, Linked List for ISR event handling.
* **STM32 Bare-Metal Development**: Register-level driver simulation and physical hardware peripherals.

---

## 🧰 Development Environment

* **Language**: C11
* **Host OS**: Ubuntu Linux (VirtualBox)
* **Target MCU**: ST NUCLEO-F401RE (ARM Cortex-M4)
* **Toolchain**: `gcc`, `arm-none-eabi-gcc`
* **Build System**: GNU Make
* **Debugging**: `gdb`, `valgrind`, OpenOCD

---

## 📁 Repository Structure

```text
Firmware-Learning/
├── 01-c-language/
│   ├── bitwise-operations/   # Bit mask & register-level bit manipulations
│   └── memory-management/    # Struct padding, pointers & dynamic memory
│
├── 02-linux/
│   ├── file-io/              # POSIX low-level system calls (open, read, write)
│   ├── process-management/   # Process lifecycle (fork, exec, wait), signals & IPC
│   └── 03-thread/            # POSIX threads, synchronization & Producer-Consumer
│
├── 03-stm32/
│   └── 01-uart-driver/       # STM32 register-level UART driver simulation
│
├── 04-data-structure/
│   └── ring-buffer/          # Pure C Ring Buffer with Function Pointer callbacks
│
├── 05-leetcode/
│   └── bit/                  # Firmware & bitwise focused algorithm practice
│
└── 06-notes/                 # Hardware notes & system architecture diagrams

🚀 Current Progress & Status
🟢 Completed / In Repository
[x] C Bitwise Operations & Register Masking

[x] C Memory Alignment & Struct Padding Analysis

[x] Linux File I/O System Calls

[x] Linux Process Creation & Pipe IPC

[x] POSIX Multithreading & Condition Variables

[x] Producer-Consumer Pattern with Shared Buffer

[x] Pure C Ring Buffer with Function Pointer Callbacks

[x] STM32 Register-level UART Driver (Linux Simulation)

🟡 In Progress
[ ] Memory Alignment & Struct Layout Deep Dive

🔵 Planned (Hardware Board On the Way)
[ ] STM32 Bare-metal GPIO Driver

[ ] STM32 Hardware UART Porting (NUCLEO-F401RE)

[ ] Hardware External Interrupts (EXTI)

[ ] UART Interrupt + Ring Buffer Integration

[ ] SPI / I2C Sensor Interfacing

[ ] Basic RTOS Integration

✒️ Engineering Principles
Decoupling: Separate lower-level drivers and upper-layer applications using Function Pointers & Callbacks.

Memory Safety: Prefer pass-by-pointer (struct*) to minimize stack copying and CPU overhead.

Clean Build: Every sub-module includes an explicit Makefile with make clean support.