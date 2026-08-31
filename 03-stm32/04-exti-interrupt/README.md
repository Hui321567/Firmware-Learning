# STM32F401RE Bare-Metal GPIO External Interrupt

## Overview

This milestone uses the NUCLEO-F401RE USER button to generate an external interrupt and toggle the onboard LD2 LED state. It extends the initial interrupt implementation into an event-driven design: the ISR publishes a button event, the main execution context consumes it and updates the LED, and the CPU waits for the next interrupt while idle. Peripheral setup and interrupt handling use direct memory-mapped register access without STM32 HAL or LL APIs.

The firmware was built with STM32CubeIDE and validated on physical NUCLEO-F401RE hardware.

## Hardware

| Function | MCU pin | Electrical behavior |
|---|---|---|
| LD2 LED | PA5 | High turns the LED on |
| USER button B1 | PC13 | Active-low; pressing creates a falling edge |

## Interrupt architecture

```text
PC13 USER button press
        |
        v
SYSCFG routes port C to EXTI13
        |
        v
EXTI13 falling-edge detection and pending flag
        |
        v
NVIC IRQ40 (ISER1 bit 8)
        |
        v
Vector table: EXTI15_10_IRQHandler
        |
        v
ISR clears EXTI_PR and sets button_event
        |
        v
Main consumes event, toggles led_state, and updates PA5
        |
        v
CPU executes WFI until the next interrupt
```

The interrupt service routine stays short: it only acknowledges EXTI13 and sets `button_event`. The main loop clears and consumes that event, toggles its non-volatile `led_state`, and updates the LED. Once no work remains, `cpu_wait_for_interrupt()` executes the `wfi` instruction instead of busy-polling.

## Register-level implementation

| Register | Purpose |
|---|---|
| `RCC_AHB1ENR` | Enables GPIOA and GPIOC peripheral clocks |
| `GPIOA_MODER` | Configures PA5 as a general-purpose output |
| `GPIOC_MODER` | Configures PC13 as an input |
| `RCC_APB2ENR` | Enables the SYSCFG peripheral clock |
| `SYSCFG_EXTICR4` | Maps EXTI13 to GPIO port C (`EXTI13 = 0b0010`) |
| `EXTI_IMR` | Unmasks EXTI line 13 |
| `EXTI_FTSR` | Enables the falling-edge trigger for EXTI13 |
| `EXTI_PR` | Reports and clears the EXTI13 pending flag |
| `NVIC_ISER1` | Enables IRQ40 using bit 8 (`40 - 32`) |
| `GPIOA_BSRR` | Atomically sets or resets PA5 |

`EXTI_PR` uses write-1-to-clear behavior. The handler therefore clears EXTI13 with:

```c
EXTI_PR = (1U << BUTTON_PIN);
```

`button_event` is declared `static volatile bool` because it is shared between the ISR and main execution contexts. `volatile` preserves the required accesses to an object that can change outside the current execution flow; it does **not** make a read-modify-write operation atomic and does not by itself prevent race conditions or provide synchronization.

`led_state` is not volatile because it belongs exclusively to the main execution context. The ISR publishes an event rather than modifying LED application state directly.

## Event-driven execution

```text
Idle in WFI
    -> PC13 falling edge wakes the CPU
    -> EXTI15_10_IRQHandler clears EXTI_PR and sets button_event
    -> main clears button_event and toggles led_state
    -> main updates LD2 through GPIOA_BSRR
    -> return to WFI
```

This separation keeps interrupt latency predictable and leaves application behavior in `main()`. It also establishes a reusable pattern for later non-blocking timing and debounce work.

## Project structure

```text
04-exti-interrupt/
|-- .project / .cproject       # STM32CubeIDE project metadata
|-- .settings/                 # Portable Eclipse/CubeIDE settings
|-- Inc/                       # Reserved include directory
|-- Src/
|   |-- main.c                 # GPIO, SYSCFG, EXTI, NVIC and ISR logic
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f401retx.s # Vector table and weak handlers
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
`-- README.md
```

Generated `Debug/` and `Release/` directories, workspace `.metadata/`, machine-local launch files, and register-view preferences are intentionally excluded.

## Build and test

1. Open STM32CubeIDE and select **File > Import > Existing Projects into Workspace**.
2. Select this directory as the project root. The imported project name is `nucleo_f401re_exti`.
3. Build the Debug configuration and confirm zero errors and zero warnings.
4. Connect the NUCLEO-F401RE through ST-Link, then start a debug session or run the firmware.
5. Press and release USER button B1 repeatedly.
6. In the debugger, confirm the CPU waits at `wfi`, a PC13 interrupt enters `EXTI15_10_IRQHandler`, `button_event` becomes true, and main consumes it.
7. Confirm that LD2 toggles once for each tested press.

The final firmware was verified on physical hardware through the complete path: WFI wait, PC13 interrupt wake-up, ISR event publication, main-context event consumption, and LED toggle.

## Debug notes

During development, EXTI13 generated its pending flag correctly and the NVIC configuration was correct, but the CPU entered `Default_Handler` instead of the intended ISR.

The handler had originally been misspelled:

```c
void EXTI15_10_IQRHandler(void);  /* Incorrect */
```

The startup vector table references the exact linker symbol `EXTI15_10_IRQHandler`. Because the misspelled function created a different symbol, it did not override the startup file's weak handler, and the vector continued to resolve to `Default_Handler`.

Correcting the function name fixed the interrupt path:

```c
void EXTI15_10_IRQHandler(void);  /* Correct */
```

This diagnosis was confirmed by checking the EXTI pending register, calculating IRQ40 as `NVIC_ISER1` bit 8, and tracing the interrupt vector entry and weak-handler symbol.

## What I learned

- How SYSCFG selects the GPIO port connected to an EXTI line.
- How falling-edge detection represents an active-low button press.
- Why EXTI pending bits must be acknowledged with write-1-to-clear semantics.
- How IRQ numbers map to NVIC set-enable registers and bit positions.
- How the vector table, linker symbols, and weak interrupt handlers work together.
- Why an ISR name must exactly match the symbol referenced by the vector table.
- How an event flag keeps the ISR short and moves application work into the main context.
- Why `volatile` does not imply atomicity, synchronization, or race-condition safety.
- How `wfi` avoids busy-polling while the firmware is idle.
