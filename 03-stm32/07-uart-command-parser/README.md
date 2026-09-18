# STM32F401RE UART Command Parser

## Overview

This milestone extends [06-uart-interrupt-ring-buffer](../06-uart-interrupt-ring-buffer/) with a bounded, line-based command parser on the NUCLEO-F401RE. USART2 reception stays in the interrupt handler; main consumes the RX ring buffer, assembles a command, and dispatches it outside interrupt context. Peripheral access uses registers without HAL or LL APIs.

Development and hardware verification took place in the local `03-stm32/06-uart-interrupt-ring-buffer` CubeIDE project. This independent archive preserves that verified `Src/main.c` unchanged and leaves the previously archived 06 milestone intact. The archived CubeIDE project is named `nucleo_f401re_uart_command_parser` to allow separate import.

## Hardware and serial settings

| Function | MCU pin | Connection / behavior |
|---|---|---|
| USART2 TX | PA2, AF7 | ST-LINK Virtual COM Port RX |
| USART2 RX | PA3, AF7 | ST-LINK Virtual COM Port TX |
| LD2 LED | PA5 | High turns LED on |

Use **9600 baud, 8 data bits, no parity, 1 stop bit, no flow control**. The inherited configuration uses `USART2_BRR = 0x683` with the default 16 MHz APB1 clock.

In the user's STM32CubeIDE Serial Console test, Enter sent `\r` (carriage return). The parser therefore terminates on `\r` and ignores `\n` (line feed). CRLF also terminates once; LF alone does not submit a command.

## Commands and responses

| Command followed by `\r` | Action | UART response |
|---|---|---|
| `LED ON` | Set PA5; LD2 on | None |
| `LED OFF` | Reset PA5; LD2 off | None |
| `STATUS` | Report parser response | `STATUS OK\r\n` |
| Unrecognized command | No LED change | `ERROR\r\n` |
| Overflowed command | Discard the entire line | None |

Matching is exact and case-sensitive using `string_equal()`. Spaces are not trimmed, and an empty command produces `ERROR`. `STATUS OK` is a fixed response, not a report of the current LED state. Backspace editing is not implemented.

## Data flow

```text
USART2 RXNE -> USART2_IRQHandler -> RX ring buffer
                                         |
                                         v
                              main: uart_rx_pop(&ch)
                                         |
                 +-----------------------+--------------------+
                 |                       |                    |
               '\r'                    '\n'               other byte
                 |                       |                    |
       if no command overflow:         ignore        if not overflowed:
       append '\0', process_cmd()                    append while index < 31
                 |                                   otherwise mark overflow
       reset index and overflow                      and discard until '\r'
```

The ISR only captures received bytes. Command assembly, comparison, GPIO actions, and blocking TX run in main. Main continuously polls the ring buffer; this milestone does not use WFI.

## Buffer ownership and overflow behavior

| Buffer | Allocation | Usable capacity | Full behavior |
|---|---|---|---|
| UART RX ring buffer | `UART_RX_BUFFER_SIZE = 8U` | 7 bytes; one slot reserved | Drop incoming byte and increment `uart_rx_overflow_count` |
| Command buffer | `CMD_BUFFER_SIZE = 32U` | 31 characters plus terminating NUL | Set `cmd_overflow`, discard remaining line, reset on CR |

The ISR owns the RX head, while main owns the RX tail. Shared RX storage, indices, and overflow counter are volatile. `cmd_buffer`, `cmd_index`, and `cmd_overflow` belong only to main.

After 31 ordinary characters, `cmd_index` is 31. The 32nd ordinary character sets `cmd_overflow = 1` without writing outside the buffer. Later ordinary characters are discarded. On `\r`, an overflowed line skips both NUL termination and `process_cmd()`, so no truncated command is executed and no new `ERROR` is transmitted. Main then resets both `cmd_index` and `cmd_overflow` to zero.

The two overflow conditions are independent. RX byte loss is counted but does not currently invalidate the command being assembled; automatic recovery from RX loss or USART hardware errors is outside this milestone. Send characters with main running when reproducing the command-buffer test, because the RX ring holds only seven bytes.

## Hardware verification

The user completed the following checks in STM32CubeIDE on physical NUCLEO-F401RE hardware before archival:

1. `LED ON`, `LED OFF`, and `STATUS` executed normally; invalid commands returned `ERROR`.
2. Serial Console Enter was observed to send `\r`, and command termination worked with that byte.
3. An overlong command produced the debugger values below before Enter:

   ```text
   cmd_index              = 31
   cmd_overflow           = 1
   uart_rx_overflow_count = 0
   ```

4. After resuming and pressing Enter, the overflowed command was discarded and parser state reset.
5. The next `STATUS` followed by Enter successfully returned `STATUS OK`.

The zero RX overflow counter isolates this observation to command-buffer overflow. These are user-reported hardware results from the development project, not a new hardware test of the archived copy.

## Project structure

```text
07-uart-command-parser/
|-- .project / .cproject       # Independent CubeIDE project name
|-- .settings/                 # Portable resource preferences
|-- Src/
|   |-- main.c                 # Verified UART, ring buffer and parser source
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f401retx.s
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
`-- README.md
```

The empty `Inc/` directory referenced by CubeIDE contains no headers and is not tracked by Git. Generated Debug/Release directories, firmware images, object files, local launch configurations, and register-view preferences are excluded by the repository `.gitignore`. Machine-specific language discovery settings are omitted from this archive.

## Build and test

1. Import this directory through **File > Import > Existing Projects into Workspace** in STM32CubeIDE.
2. Select `nucleo_f401re_uart_command_parser` and build the Debug configuration.
3. Connect the board through ST-LINK, select this project's ELF in a new debug configuration, and run it.
4. Open the Virtual COM Port with the serial settings above.
5. Send each supported command followed by Enter; verify the LED or response.
6. Send an invalid command and check `ERROR`.
7. With main running, send more than 31 ordinary characters. Inspect the three overflow values above before Enter; avoid receiving further characters while halted.
8. Resume, press Enter, then send `STATUS` and Enter. Confirm `STATUS OK` after recovery.

The inherited source includes an FPU-initialization warning when compiled with the CubeIDE hard-float configuration. This integer-only milestone does not add FPU initialization or change the verified startup behavior.

## What I learned

- How to separate byte reception, buffering, line assembly, and command dispatch.
- Why a C string buffer needs one reserved byte for its terminating NUL.
- How actual terminal line endings affect parser behavior.
- Why overflow handling must discard a complete command rather than execute a truncated prefix.
- How to verify recovery on the next command and distinguish command overflow from RX overflow.
