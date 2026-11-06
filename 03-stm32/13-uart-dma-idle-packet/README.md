# STM32F401RE USART2 DMA Circular + IDLE Packet Parser

## Overview

This milestone extends [12-uart-rx-dma](../12-uart-rx-dma/) with USART IDLE notifications, a CR/LF-terminated bounded packet parser, a software overflow check, and a fault-triggered receive-session restart. The project targets NUCLEO-F401RE using direct register programming without HAL or LL APIs.

The original code built in STM32CubeIDE with **0 errors and 0 warnings**. The user observed RX, parser, and debugger-injected software-fault recovery tests on hardware. This independent archive uses CubeIDE metadata adapted from milestone 12; the archived copy has **not** been freshly imported or built.

## Hardware and serial settings

| Item | Configuration |
|---|---|
| Board / MCU | NUCLEO-F401RE / STM32F401RE (Cortex-M4) |
| USART RX | USART2, PA3 AF7, ST-LINK Virtual COM Port |
| Serial format | 115200 baud, 8N1, no flow control |
| Clock assumption | PCLK1 = 16 MHz; BRR = `0x008B` |
| DMA | DMA1 Stream5 Channel4; peripheral-to-memory |
| DMA settings | Circular, 128-byte buffer, memory increment, 8-bit, direct mode |
| Packet parser | Maximum 32 content bytes; CR or LF terminator |

The tested STM32CubeIDE Serial Console sent carriage return (`\r`) on Enter. Both CR and LF terminate nonempty packets; CRLF does not create an extra packet.

## Data flow

~~~text
PC Serial Console --> USART2 RX (PA3) --> USART2_DR
                                              |
                                              v DMAR
                                  DMA1 Stream5 Channel4
                                              |
                                              v
                                     rx_buffer[128]
                                              |
                            DMA HT/TC or USART IDLE IRQ
                                              |
                               rx_data_pending event
                                              |
                                              v
                              main: reconstruct RX total
                                              |
                               consume unread DMA bytes
                                              |
                                              v
                               packet_parser_push(byte)
                                              |
                               CR / LF packet terminator
                                              |
                                              v
                                 last_packet and counters
~~~

The ISRs record events and faults. The main loop consumes bytes, parses packets, and performs recovery.

## Register-level implementation

| Register | Role |
|---|---|
| `RCC_AHB1ENR` / `RCC_APB1ENR` | Enable GPIOA, DMA1 and USART2 clocks |
| `GPIOA_MODER` / `GPIOA_AFRL` | PA3 alternate function AF7 |
| `USART2_BRR` / `USART2_CR1` / `CR3` | Baud rate, UE/RE/IDLEIE, DMAR/EIE |
| `USART2_SR` / `USART2_DR` | Receive status and error/IDLE acknowledgement |
| `DMA1_S5CR` / `S5FCR` | Channel, circular/direct mode and interrupts |
| `DMA1_S5NDTR` / `S5PAR` / `S5M0AR` | Transfer count and RX addresses |
| `DMA1_HISR` / `DMA1_HIFCR` | DMA Stream5 interrupt flags |
| `NVIC_ISER` / `ICER` / `ICPR` | Enable, mask and clear pending IRQ16 / IRQ38 |

## Packet parser

- `packet_parser_push()` consumes bytes in main, never inside an ISR.
- CR (`\r`) and LF (`\n`) terminate packets; empty packets are ignored.
- Up to 32 bytes of contents are accepted and NUL-terminated in `last_packet`.
- A 33rd content byte marks the packet too long; remaining input is discarded until a terminator.
- `packet_complete_count` and `packet_too_long_count` record separate events.
- `PING` and `STATUS` are **test strings**, not implemented TX commands; there is no PONG response.

## Software fault recovery

USART ORE/NE/FE and DMA TE are monitored by their respective handlers. A best-effort buffer overflow check compares received and processed totals. These paths set `usart_rx_fault` and main runs `uart_dma_rx_recover()`.

1. Mask DMA and USART NVIC interrupts.
2. Disable USART DMAR to stop incoming DMA requests.
3. Disable DMA Stream5; wait until its EN bit reads 0.
4. Reinitialize USART2 and DMA1 Stream5.
5. Reset the receive-session positions and counts; discard any incomplete parser packet.
6. Clear pending NVIC interrupts and restart DMA/USART reception.

The previous session's in-flight data is discarded rather than recovered. Historical diagnostic variables are retained where implemented.

## Test results

| Scenario | Observed result |
|---|---|
| DMA RX lengths: 5, 64, 128, 129 bytes | Previously tested successfully |
| `PING` + Enter | 1 completed packet, `last_packet="PING"`, 5 bytes |
| `PING` then `STATUS` | 2 packets, `last_packet="STATUS"`, 12 bytes |
| Fragmented `PI` + `NG` + Enter | One `PING` packet, 5 bytes |
| 33 characters + Enter; then `PING` | 1 overlong packet and 1 valid `PING`, 39 bytes |
| Debugger sets `usart_rx_fault=1` | `dma_recovery_count=1`, Fault cleared, Stream5 EN=1 |
| Recovery followed by `PING` | `PING` parsed after restart, 5-byte new RX session |
| Partial `PI` → Fault → `PING` | New packet is `PING`, not `PIPING` |

These are results from the original development project observed in STM32CubeIDE Debug Expressions. Physical USART/DMA error injection and exhaustive sustained-load tests have not been conducted.

## Project structure

~~~text
13-uart-dma-idle-packet/
|-- .project / .cproject       # Independent CubeIDE project
|-- .settings/                 # Portable settings from milestone 12
|-- Src/
|   |-- main.c                # Bare-metal USART/DMA, IDLE, parser, recovery
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f401retx.s
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
`-- README.md
~~~

CubeIDE project name: `STM32_UART_DMA_IDLE_PACKET`. Build output and machine-specific debug configurations are excluded by the repository `.gitignore`.

## Build and test

1. In STM32CubeIDE choose **File → Import → Existing Projects into Workspace** and select this independent directory.
2. Build `STM32_UART_DMA_IDLE_PACKET` in Debug mode. The archived project metadata was adapted from milestone 12 and should be freshly verified on import.
3. Connect the NUCLEO-F401RE via ST-LINK and open its Virtual COM Port at **115200 8N1**.
4. Start firmware, send `PING` followed by Enter, and inspect `last_packet` and `packet_complete_count`.
5. Relevant Debug Expressions: `processed_count`, `dma_total_received`, `dma_tc_count`, `usart_idle_count`, `usart_rx_fault`, `dma_recovery_count`.

## Known limitations

- TC interrupt counts + NDTR are **not** a lossless absolute DMA producer count. Coalesced/missed TC events or wrap races can undermine the overflow estimate.
- DMA Direct Mode Error (DMEIF) and FIFO Error (FEIF) have no complete runtime handling.
- The successful Recovery tests injected a **software** Fault. Physical hardware error recovery was not comprehensively tested.
- The DMA-disable EN wait has no timeout. A persistent fault can lead to repeated recovery attempts.
- There is no TX command-response protocol, flow control, or replay of data lost during recovery.
- The baud-rate divider assumes PCLK1 remains 16 MHz. Main busy-polls; maximum sustainable throughput was not measured.

## What I learned

- How circular DMA differs from an interrupt-driven software ring buffer.
- Why a USART IDLE event is not necessarily an application packet boundary.
- How short ISRs can notify main while parsing remains outside interrupt context.
- Why DMA EN polling, flag clearing, and peripheral restart must be ordered.
- Why debugger-simulated Fault recovery is not identical to real hardware error recovery.

## References

- ST RM0368 — STM32F401 Reference Manual
- ST UM1724 — Nucleo-64 Boards User Manual
