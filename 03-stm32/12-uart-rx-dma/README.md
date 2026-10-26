# STM32F401RE USART2 RX DMA Bare-Metal

## Overview

**Status: Core Features Completed**

This milestone runs on the NUCLEO-F401RE and receives USART2 data using DMA1 Stream5, Channel 4, without HAL or LL APIs. The final program uses a 128-byte circular buffer, polls the DMA remaining count in the main loop, and records half-transfer, transfer-complete, transfer-error, and USART error diagnostics.

The original Bare-Metal C implementation and portable STM32CubeIDE project files are preserved from C:/STM32/workspace/STM32_UART_RX_DMA. Core reception is complete; guaranteed DMA buffer-overrun detection and automatic error recovery remain outside the completed scope.

## Hardware

| Function | MCU / board pin | Configuration |
|---|---|---|
| USART2 RX | PA3 | AF7, receive only |
| Serial input | ST-Link virtual COM port or 3.3 V USB-UART TX | 115200 baud, 8N1, no flow control |
| Reference | GND | Common ground when using an external adapter |

Use one input source at a time. The program does not configure USART TX or echo received bytes. Observe reception using debugger variables. Timing assumes the reset PCLK1 of 16 MHz; BRR = 0x008B gives approximately 115108 baud.

## Architecture

~~~text
Serial sender -> USART2 RX / DR -> DMA1 Stream5 Channel 4
                                         |
                                  rx_buffer[128]
                                         |
                              main loop samples NDTR
                                         |
                       write_pos = (128 - NDTR) % 128
                                         |
                     consume read_pos up to sampled write_pos
                                         |
                          last_byte / processed_count

DMA HT/TC/TE interrupt -> diagnostic counters
USART ORE/NE/FE interrupt -> latch errors -> stop RX -> fault state
~~~

DMA writes bytes independently of CPU execution. The main loop consumes data up to one sampled producer position, advances read_pos modulo RX_SIZE, and increments processed_count. The next iteration obtains a fresh NDTR snapshot. HT/TC interrupt handlers record events; they do not copy or process the buffer.

The application retains only last_byte and a count of consumed bytes, not an application-level copy of the complete received stream. volatile makes accesses observable to the compiler; it does not prevent DMA from overwriting unread data.

## Register Configuration

| Register | Configuration / purpose |
|---|---|
| RCC_AHB1ENR | GPIOAEN bit 0; DMA1EN bit 21 |
| RCC_APB1ENR | USART2EN bit 17 |
| GPIOA_MODER / GPIOA_AFRL | PA3 alternate-function mode, AF7 |
| USART2_BRR | 0x008B; PCLK1 assumed to be 16 MHz |
| USART2_CR1 / CR2 | RE and UE enabled; default 8-bit word, no parity, one stop bit |
| USART2_CR3 | DMAR bit 6 and EIE bit 0 enabled |
| DMA1_S5CR | CHSEL=4, peripheral-to-memory, CIRC, MINC, byte-sized peripheral/memory transfers; HTIE, TCIE, TEIE enabled |
| DMA1_S5FCR | 0: direct mode; FIFO error interrupt disabled |
| DMA1_S5PAR | USART2_DR address 0x40004404 |
| DMA1_S5M0AR | Address of rx_buffer[0] |
| DMA1_S5NDTR | Initially 128; decremented and reloaded by DMA in circular mode |
| DMA1_HISR / HIFCR | Observe and clear Stream5 interrupt flags |
| NVIC_ISER0 | IRQ16: DMA1_Stream5_IRQHandler |
| NVIC_ISER1 | Bit 6 / IRQ38: USART2_IRQHandler |

Initialization disables USART2, clears its prior receive status through SR then DR reads, disables the DMA stream and waits for EN to clear, configures the stream, clears its flags, and enables DMA. Main then enables both IRQs, DMAR, receiver/USART, and USART error interrupts.

## Interrupt Handling

DMA1_Stream5_IRQHandler snapshots HISR once and independently checks HTIF5, TCIF5, and TEIF5. Each observed flag is cleared through HIFCR and increments its corresponding counter: dma_ht_count, dma_tc_count, or dma_te_count. Multiple flags in one snapshot can all be handled. A transfer error is counted; no stream restart is implemented.

USART2_IRQHandler snapshots SR and masks ORE, NE, and FE. On an error it reads DR to complete the status-clear sequence, ORs the observed errors into usart_error_flags, increments usart_ore_count for ORE, clears EIE and DMAR, disables RE, and sets usart_rx_fault. The main loop stops consuming bytes while faulted. The handler does not explicitly disable the DMA stream, reinitialize positions, or restart reception.

RXNE interrupts and IDLE-line framing are not enabled. Parity diagnostics are not implemented. Interrupt counters describe serviced status events, not a guaranteed total of every physical event.

## Test Results

The user reported completion of the following hardware tests before archival. The available conversation preview did not expose detailed measurements, payloads, or screenshots. Results below preserve the reported scope without inventing exact counter values.

| Test | Reported result | Interpretation / observation points |
|---|---|---|
| 16 Bytes reception | Completed | Short-transfer reception and consumption; inspect rx_buffer, last_byte, processed_count |
| 128 Bytes reception | Completed | Full-buffer-length reception test; archived RX_SIZE is 128 |
| HT / TC | Completed | Half-transfer and transfer-complete diagnostics integrated; inspect dma_ht_count and dma_tc_count |
| Circular wraparound | Completed | Reception across buffer wrap exercised; inspect NDTR and modulo read/write positions |
| Error-diagnostic integration | Completed | DMA TE and USART ORE/NE/FE diagnostic paths integrated; inspect counters, latched flags, and fault state |

The 16 Bytes entry records a test size; the retained source uses RX_SIZE=128. Earlier buffer-size variants are not reconstructed. For the final 128-byte configuration, nominal HT and TC boundaries are 64 and 128 incoming bytes respectively. These boundaries are expected behavior from the configuration, not recovered measurements.

Diagnostic integration does not establish that every individual hardware error was deliberately injected. These tests do not establish lossless operation while the consumer stalls, exhaustive error detection, or successful error recovery.

### Archive rebuild verification

On 2026-10-26, the copied C sources and startup assembly were compiled and linked with GNU Tools for STM32 14.3.rel1, Cortex-M4, GNU C11, -O0, -Wall, hard-float target options, nano/nosys specs, and the archived FLASH linker script. The compiler returned exit code 0 with no diagnostics.

This was a command-line rebuild of the archived files, not a new CubeIDE import or hardware execution. Original source and project-file hashes were checked separately. No new flashing, serial transmission, or fault injection was performed during archival.

## Project structure

~~~text
12-uart-rx-dma/
|-- .project / .cproject       # Original project name: STM32_UART_RX_DMA
|-- .settings/                # Portable CubeIDE settings
|-- Inc/                      # Reserved include directory (may be empty)
|-- Src/
|   |-- main.c                # USART2, circular DMA, consumer, diagnostics
|   |-- syscalls.c
|   '-- sysmem.c
|-- Startup/
|   '-- startup_stm32f401retx.s
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
'-- README.md
~~~

Generated build files, binaries, personal launch files, SFR preferences, and workspace metadata are excluded. The repository directory follows the numbered milestone convention; the original CubeIDE project name remains STM32_UART_RX_DMA.

## Build and test

1. In STM32CubeIDE, select File > Import > Existing Projects into Workspace and select this directory.
2. Use a separate workspace if STM32_UART_RX_DMA is already imported. Build the Debug configuration.
3. Connect the NUCLEO-F401RE through ST-Link and configure the serial sender for 115200 baud, 8N1, no flow control. Disable automatic CR/LF insertion when checking exact byte counts.
4. Run the firmware and send a known 16-byte payload. Inspect processed_count and last_byte after allowing the main loop to run.
5. Reset between independent tests. Send 128 bytes, then exercise repeated traffic across wraparound. Record payload, counters, positions, and error flags.
6. Observe dma_ht_count / dma_tc_count and usart_rx_fault. A fault stops further software consumption; reset/restart the test to return to a known state.

Avoid treating a halted CPU with incoming serial traffic as a lossless test. Debugger peripheral reads can affect USART status; inspect software diagnostic variables first.

## Known Limitations

- **DMA buffer overrun cannot be guaranteed to be fully detected.** Modulo producer and consumer positions cannot distinguish an empty buffer from one or more complete producer laps. Unread bytes may be silently overwritten.
- HT/TC flags can remain set across repeated events and serviced interrupt counts may coalesce. The current main loop does not combine these counters into an absolute producer sequence. HT/TC counts and USART ORE do not prove absence of DMA buffer overwrite.
- **Error recovery is not implemented.** USART errors latch a fault and stop RX/consumption; DMA TE is counted without restart. There is no automatic reinitialization, resynchronization, or replay.
- USART ORE concerns the peripheral receive path; it is distinct from DMA overwriting unread SRAM buffer data.
- SR/DR reads during fault handling may consume data and do not preserve a complete error-associated stream.
- FIFO/direct-mode errors are not fully diagnosed; their flags are cleared at initialization but no dedicated handling or interrupts are enabled for them.
- No backpressure, hardware flow control, packet framing, IDLE-line handling, message queue, or application parser is provided.
- A sampled write position becomes stale while DMA continues writing. Processing must keep up; no measured maximum sustainable throughput or worst-case interrupt latency is claimed.
- The stream-disable wait has no timeout. Counters can wrap. The main loop busy-polls, including while faulted.
- Baud configuration assumes PCLK1=16 MHz. Clock changes require a matching BRR calculation.
- Startup and hard-float project settings are preserved. This integer-only build does not establish safe floating-point use or complete FPU initialization.

## What I learned

- DMA moves receive data without one CPU interrupt per byte.
- NDTR exposes remaining transfers and supports modulo producer-position calculation.
- Circular DMA wraparound and application consumption are independent.
- Interrupt status flags are notifications rather than a lossless event history.
- Peripheral overrun, SRAM buffer overwrite, diagnostics, and recovery are separate concerns.
