# STM32F401RE USART2 RX Interrupt + Ring Buffer

## Overview

This milestone ports UART communication to the NUCLEO-F401RE and connects interrupt-driven USART2 reception to a small ring buffer. Peripheral setup, the interrupt handler, the buffer, and LED control use direct register access without STM32 HAL or LL APIs.

Each received byte is captured by `USART2_IRQHandler()` and pushed into the RX ring buffer. The main loop removes bytes in FIFO order and handles application commands outside interrupt context. Sending `1` turns LD2 on and returns `LED ON`; sending `0` turns LD2 off and returns `LED OFF`.

The project was built with STM32CubeIDE with zero errors and verified on physical NUCLEO-F401RE hardware. USART2 RX interrupts, FIFO ordering, LED commands, and the overflow counter were all observed successfully.

## Hardware and serial settings

| Function | MCU pin | NUCLEO connection |
|---|---|---|
| USART2 TX | PA2, AF7 | ST-LINK Virtual COM Port RX |
| USART2 RX | PA3, AF7 | ST-LINK Virtual COM Port TX |
| LD2 LED | PA5 | High turns the LED on |

Serial configuration:

- Baud rate: 9600 baud
- Data bits: 8
- Parity: none
- Stop bits: 1
- Flow control: none
- Line ending: optional for commands; only the `1` and `0` bytes affect the LED

With the default 16 MHz APB1 peripheral clock and 16x oversampling, `USART2_BRR = 0x683` selects approximately 9600 baud.

## Data flow

```text
Serial terminal sends a byte
          |
          v
PA3 / USART2 RX receives the byte
          |
          v
RXNE becomes set and IRQ38 is raised
          |
          v
USART2_IRQHandler reads USART2_DR
          |
          v
ISR pushes the byte into uart_rx_buffer
          |
          +-- buffer has room: advance uart_rx_head
          |
          `-- buffer full: increment uart_rx_overflow_count
          |
          v
main calls uart_rx_pop()
          |
          +-- '1': set PA5 and transmit "LED ON"
          |
          +-- '0': reset PA5 and transmit "LED OFF"
          |
          `-- other byte: consume without changing the LED
```

Reception is interrupt-driven, while command processing and response transmission remain in the main context. This keeps the RX interrupt handler short and prevents LED application logic or blocking TX waits from running inside the ISR.

## Ring-buffer behavior

The RX buffer uses `uart_rx_head` as the ISR-owned producer index and `uart_rx_tail` as the main-owned consumer index. Both indices and the shared storage are `volatile` because they are accessed across interrupt and main contexts.

`UART_RX_BUFFER_SIZE` is 8. The implementation reserves one slot to distinguish full from empty, so its usable capacity is 7 bytes:

```c
next_head = (uart_rx_head + 1U) % UART_RX_BUFFER_SIZE;

if (next_head != uart_rx_tail)
{
    /* space is available */
}
else
{
    uart_rx_overflow_count++;
}
```

When the buffer is full, the newly received byte is dropped and `uart_rx_overflow_count` is incremented. Existing unread bytes are preserved. The counter makes overload visible in the debugger instead of silently hiding it.

This is a single-producer/single-consumer design: only the USART2 ISR advances `uart_rx_head`, and only main advances `uart_rx_tail`. The Cortex-M4 can perform the 8-bit index accesses atomically. The implementation does not provide a general multi-producer or multi-consumer queue.

## Register-level implementation

| Register | Purpose |
|---|---|
| `RCC_AHB1ENR` | Enables the GPIOA clock |
| `RCC_APB1ENR` | Enables the USART2 clock |
| `GPIOA_MODER` | Configures PA2/PA3 for alternate function and PA5 as output |
| `GPIOA_AFRL` | Selects AF7 for USART2 TX and RX |
| `GPIOA_BSRR` | Atomically sets or resets the PA5 LED output |
| `USART2_SR` | Reports TXE and RXNE status |
| `USART2_DR` | Transmits and receives UART data |
| `USART2_BRR` | Configures the baud-rate divider |
| `USART2_CR1` | Selects 8N1-related options and enables UE, TE, RE, and RXNEIE |
| `USART2_CR2` | Selects one stop bit |
| `NVIC_ISER1` | Enables USART2 IRQ38 with bit 6 (`38 - 32`) |

Reading `USART2_DR` in the RXNE handler consumes the received byte and clears the receive-not-empty condition for the normal receive path.

## Commands and observed results

| Input byte | LED action | UART response |
|---|---|---|
| `1` | PA5 set; LD2 on | `LED ON\r\n` |
| `0` | PA5 reset; LD2 off | `LED OFF\r\n` |
| Any other byte | No LED change | No response |

Hardware verification covered:

1. USART2 RXNE entered the correctly named `USART2_IRQHandler` from the startup vector table.
2. Received bytes were pushed by the ISR and later popped by main in FIFO order.
3. Sending `1` turned LD2 on and produced the expected response.
4. Sending `0` turned LD2 off and produced the expected response.
5. Filling the buffer faster than main consumed it incremented `uart_rx_overflow_count`.

## Project structure

```text
06-uart-interrupt-ring-buffer/
|-- .project / .cproject       # STM32CubeIDE project metadata
|-- .settings/                 # Portable Eclipse/CubeIDE settings
|-- Inc/                       # Reserved include directory
|-- Src/
|   |-- main.c                 # USART2, RX ISR, ring buffer and LED commands
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f401retx.s # Vector table and weak handlers
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
`-- README.md
```

Generated `Debug/` and `Release/` directories, ELF/BIN/HEX files, object and dependency files, machine-local launch configurations, and register-view preferences are excluded by the repository `.gitignore` and are not part of this archive.

## Build and test

1. Open STM32CubeIDE and select **File > Import > Existing Projects into Workspace**.
2. Select this directory as the project root. The CubeIDE project name is `nucleo_f401re_uart_tx`.
3. Build the Debug configuration and confirm zero errors.
4. Connect the NUCLEO-F401RE through ST-LINK and run the firmware.
5. Open the ST-LINK Virtual COM Port at 9600 8N1.
6. Send `1` and `0`, then confirm LD2 and the returned status messages.
7. To inspect buffering, watch `uart_rx_head`, `uart_rx_tail`, `uart_rx_buffer`, and `uart_rx_overflow_count` in the debugger.

## What I learned

- How PA2 and PA3 are routed to USART2 through GPIO alternate function 7.
- How RXNE and NVIC IRQ38 provide interrupt-driven byte reception.
- Why an ISR should capture data quickly and defer application work to main.
- How a single-producer/single-consumer ring buffer decouples interrupt timing from application timing.
- Why a reserved slot distinguishes the full and empty states.
- How an overflow counter exposes dropped input during receive bursts.
