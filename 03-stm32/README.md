# STM32 Bare-Metal & Driver Development

## 1. Overview
This module focuses on low-level firmware development for STM32 microcontrollers (ARM Cortex-M4 Architecture, targeted at NUCLEO-F401RE), without relying on vendor-provided HAL abstractions. It emphasizes direct memory-mapped register manipulation, startup scripts, linker scripts, and hardware interrupt handling.

## 2. Learning Objectives
- Master ARM Cortex-M4 bare-metal programming using pure C structures and pointer dereferencing.
- Understand the build toolchain flow: compilation with `arm-none-eabi-gcc`, linking with GNU Linker Scripts (`.ld`), and binary generation.
- Implement register-level drivers for GPIO, Timers, UART, and Interrupt Controller (NVIC).
- Integrate hardware UART RX interrupts with Ring Buffers for efficient asynchronous communication.

## 3. Key Concepts
- **Cross-Compilation**: Compiling ARM machine code binaries on an x86_64 Linux host using `arm-none-eabi-gcc`.
- **Memory-Mapped I/O**: Accessing peripheral hardware registers by dereferencing specific physical memory addresses.
- **Linker Script (`.ld`)**: Defining Flash and SRAM memory boundaries, section placement (`.text`, `.data`, `.bss`), and entry point vectors.
- **Interrupt Service Routine (ISR)**: Asynchronous event handlers triggered by hardware interrupts.

## 4. Environment Setup
- **Cross Compiler**: `arm-none-eabi-gcc`
- **Flashing & Debugging**: OpenOCD / ST-Link V2
- **Hardware Target**: NUCLEO-F401RE (STM32F401RE) / CP2102 USB-TTL UART Module

## 5. Project Structure
```text
03-stm32/
├── 01-uart-driver/        # Simulated UART driver logic in C
├── 01-gpio-baremetal/     # Bare-metal GPIO LED control (In progress)
└── README.md
```
## 6. Build & Flash Workflows
```Bash
# Verify ARM Toolchain
arm-none-eabi-gcc --version

# Build Bare-metal binary
make

# Flash firmware via OpenOCD
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/firmware.elf verify reset exit"
```

## 7. Verification / Debugging
1. Toolchain Verification: Verified arm-none-eabi-gcc cross-compiler environment on Linux host.

2. GDB & OpenOCD Debugging: Connects to target MCU via GDB server for single-step register-level debugging.

## 8. What I Learned
- The fundamental difference between Native Compilation (Host) and Cross Compilation (Target).

- Why register addresses require volatile type qualifiers to prevent compiler optimization on memory-mapped I/O.