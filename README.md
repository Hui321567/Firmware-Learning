# Firmware Learning Journey 🚀

A hands-on repository focused on embedded firmware and system software engineering.

This repository documents my progression from low-level C and Linux system programming to ARM Cortex-M bare-metal development and real-time data structures.

## Learning focus

- **Core C and memory:** bitwise operations, pointer arithmetic, memory alignment, and struct padding.
- **Linux system programming:** file descriptors, system calls, process management, signals, and IPC.
- **Concurrency:** POSIX threads, mutexes, condition variables, and Producer-Consumer patterns.
- **Embedded data structures:** ring buffers and callback-based interfaces.
- **STM32 bare-metal development:** direct register access and physical hardware verification.

## Development environment

- Language: C11
- Host OS: Ubuntu Linux and Windows
- Target: ST NUCLEO-F401RE (Arm Cortex-M4)
- Toolchains: `gcc`, `arm-none-eabi-gcc`, STM32CubeIDE
- Debugging/flashing: GDB, OpenOCD, ST-Link

## Repository structure

```text
Firmware-Learning/
|-- 01-c-language/
|   |-- bitwise-operations/
|   `-- memory-management/
|-- 02-linux/
|   |-- file-io/
|   |-- process-management/
|   |-- 03-thread/
|   `-- mini-shell/
|-- 03-stm32/
|   |-- 01-uart-driver/          # Register-level UART driver simulation
|   |-- 02-gpio-baremetal/      # PA5 LED blink / GPIO output milestone
|   |-- 03-gpio-button-polling/ # PC13 polling, toggle and debounce milestone
|   |-- 04-exti-interrupt/      # PC13 EXTI13 / IRQ40 interrupt milestone
|   |-- 05-systick-nonblocking-debounce/ # EXTI + 1 ms SysTick debounce
|   |-- 06-uart-interrupt-ring-buffer/   # USART2 RX interrupt + FIFO buffering
|   |-- 07-uart-command-parser/          # Line commands + overflow recovery
|   |-- 08-tim2-hardware-timer/          # TIM2 update interrupt milestone
|   `-- 09-tim2-pwm/                    # TIM2_CH1 / PA5 AF1, 1 kHz, 50% duty
|-- 04-data-structure/
|   `-- ring-buffer/
|-- 05-leetcode/
|   `-- bit/
`-- 06-notes/
```

## Current progress

### Completed

- [x] C bitwise operations and register masking
- [x] C memory alignment, struct padding, and dynamic memory exercises
- [x] Linux file I/O system calls
- [x] Linux process creation, signals, and pipe IPC
- [x] POSIX threads, synchronization, and Producer-Consumer pattern
- [x] Pure C ring buffer with callback interfaces
- [x] STM32 register-level UART driver simulation
- [x] STM32F401RE bare-metal GPIO output / LED blink (hardware validated)
- [x] STM32F401RE button polling / LED toggle / debounce (hardware validated)
- [x] STM32F401RE EXTI button interrupt / LED state toggle (hardware validated)
- [x] STM32F401RE SysTick non-blocking button debounce (hardware validated)
- [x] STM32F401RE USART2 RX interrupt / ring buffer / LED commands (hardware validated)
- [x] STM32F401RE UART command parser / command overflow recovery (hardware validated)
- [x] STM32F401RE TIM2 hardware timer / update interrupt (hardware validated)
- [x] STM32F401RE TIM2_CH1 hardware PWM / PA5 AF1, 1 kHz, 50% duty (hardware validated)

### Planned

- [ ] USART error handling and interrupt- or DMA-driven transmission
- [ ] SPI / I2C sensor interfacing
- [ ] Basic RTOS integration

## Engineering principles

- **Direct understanding:** learn peripheral behavior at register level before adding abstractions.
- **Decoupling:** separate low-level drivers from application logic using clear interfaces and callbacks.
- **Memory safety:** use explicit ownership and avoid unnecessary copies.
- **Clean builds:** keep generated artifacts out of version control and make projects reproducible.
