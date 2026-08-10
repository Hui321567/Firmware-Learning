# STM32 Bare-Metal UART Driver Simulation

This repository contains a modular, bare-metal UART driver abstraction for STM32 microcontrollers, simulated and tested in a Linux C environment.

## Key Features

1. **Memory-Mapped Register Struct**:
   - Maps USART hardware registers (`SR`, `DR`, `BRR`, `CR1`, `CR2`, `CR3`) using a `volatile` C `struct`.
2. **Bitwise Register Control**:
   - Configures status and control flags using standard bit-mask operations (`USART_SR_TXE`, `USART_CR1_TE`, `USART_CR1_RE`, `USART_CR1_UE`).
3. **Polling-based I/O**:
   - Implements blocking polling mechanism for transmitting characters and strings (`uart_send_char`, `uart_send_string`).

## File Structure

- `uart.h`: Register structure definitions, bit masks, and API prototypes.
- `uart.c`: Implementation of UART initialization and polling transmit/receive functions.
- `main.c`: Test entry point demonstrating driver initialization and output.
- `Makefile`: Build system setup.

## Build & Run

```bash
# Compile the driver demo
make

# Run the test program
./uart_demo

# Clean build artifacts
make clean