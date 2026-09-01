# STM32F401RE EXTI + SysTick Non-Blocking Debounce

## Overview

This milestone extends the EXTI event-driven button project with a 1 ms SysTick timebase and a 50 ms non-blocking debounce state. It runs on the NUCLEO-F401RE and uses direct register access without STM32 HAL or LL APIs.

The EXTI handler acknowledges the interrupt and publishes an event only. Main starts one debounce interval, continues running instead of busy-waiting, and checks the active-low PC13 input after 50 ms. A confirmed press toggles the PA5 LED state. The CPU executes `WFI` whenever the current loop has no more work; SysTick wakes it every 1 ms while a debounce interval is being measured.

This exact control flow was built, run, and stepped through successfully on physical hardware.

## Hardware

| Function | MCU pin | Electrical behavior |
|---|---|---|
| LD2 LED | PA5 | High turns the LED on |
| USER button B1 | PC13 | Active-low; a press produces a falling edge |

The SysTick reload value is `15999`, which produces a 1 ms interrupt when the core clock is 16 MHz:

```text
16,000,000 / (15,999 + 1) = 1,000 Hz
```

## Control flow

```text
PC13 HIGH -> LOW
       |
       v
EXTI13 falling-edge interrupt
       |
       v
ISR clears EXTI_PR and sets button_event
       |
       v
main consumes the event
       |
       v
if no debounce is active:
debounce_pending = true
debounce_start_ticks = system_ticks
       |
       v
main continues; WFI waits between interrupts
       |
       v
SysTick increments system_ticks every 1 ms
       |
       v
50 ms elapsed (wrap-safe unsigned subtraction)
       |
       v
read PC13 through GPIOC_IDR
       |
       +-- HIGH: reject as noise
       |
       `-- LOW: toggle led_state and update PA5
```

The elapsed-time check is deliberately outside the `if (button_event)` block. The initial EXTI event starts the timer, while later main-loop iterations must be able to observe the 50 ms expiry even when no new button event arrives.

While `debounce_pending` is true, bounce-generated EXTI events are acknowledged but do not restart `debounce_start_ticks`. Once the contact is held steadily low, no more falling edges occur. Button release is a rising edge and is not enabled in this milestone.

## Shared state and interrupt behavior

- `button_event` is `volatile` because EXTI writes it and main reads it.
- `system_ticks` is `volatile` because SysTick writes it and main reads it.
- `led_state`, `debounce_pending`, and `debounce_start_ticks` belong to main only and therefore are not volatile.
- `EXTI_PR` is write-1-to-clear; the handler acknowledges line 13 with `EXTI_PR = (1U << BUTTON_PIN)`.
- `button_is_pressed()` hides the active-low register test but preserves the verified `GPIOC_IDR` logic.
- Unsigned subtraction in `system_ticks - debounce_start_ticks` remains correct across a normal 32-bit tick wraparound.

## Register-level implementation

| Register | Purpose |
|---|---|
| `RCC_AHB1ENR` | Enables GPIOA and GPIOC clocks |
| `GPIOA_MODER` / `GPIOA_BSRR` | Configures and atomically controls PA5 |
| `GPIOC_MODER` / `GPIOC_IDR` | Configures PC13 input and reads its current level |
| `RCC_APB2ENR` / `SYSCFG_EXTICR4` | Enables SYSCFG and routes port C to EXTI13 |
| `EXTI_IMR` / `EXTI_FTSR` / `EXTI_PR` | Unmasks falling-edge EXTI13 and acknowledges pending events |
| `NVIC_ISER1` | Enables IRQ40 using bit 8 (`40 - 32`) |
| `SYSTICK_LOAD` / `SYSTICK_VAL` / `SYSTICK_CTRL` | Configures the 1 ms core timer interrupt |

## Debug verification

The full path was verified in STM32CubeIDE on the NUCLEO-F401RE:

1. PC13 falling edge entered `EXTI15_10_IRQHandler`.
2. The ISR cleared EXTI13 pending state and set `button_event`.
3. Main consumed the event, set `debounce_pending`, and captured `debounce_start_ticks`.
4. In one measured run, `debounce_start_ticks` was `2937` and the expiry breakpoint was reached at `system_ticks = 2987`.
5. `2987 - 2937 = 50`, confirming the 50 ms non-blocking interval.
6. PC13 was still low, so `led_state` changed from `false` to `true` and `led_on()` drove PA5 high.
7. `button_event` and `debounce_pending` returned to `false` after the event was consumed and confirmed.

The values `2937` and `2987` are one debugger capture, not fixed runtime values.

## Project structure

```text
05-systick-nonblocking-debounce/
|-- .project / .cproject       # STM32CubeIDE project metadata
|-- .settings/                 # Portable Eclipse/CubeIDE settings
|-- Inc/                       # Reserved include directory
|-- Src/
|   |-- main.c                 # GPIO, EXTI, SysTick, debounce and LED logic
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f401retx.s # Vector table and weak handlers
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
`-- README.md
```

Generated `Debug/` and `Release/` directories, object files, firmware images, workspace metadata, machine-local launch files, and register-view preferences are excluded by the repository `.gitignore`.

## Build and test

1. Open STM32CubeIDE and select **File > Import > Existing Projects into Workspace**.
2. Select this directory as the project root. The imported project name is `nucleo_f401re_systick_debounce`.
3. Build the Debug configuration and confirm zero errors and zero warnings.
4. Connect the NUCLEO-F401RE through ST-Link and run or debug the firmware.
5. Press USER button B1 and confirm LD2 toggles once after the 50 ms confirmation interval.
6. Optionally watch `button_event`, `debounce_pending`, `debounce_start_ticks`, `system_ticks`, and `led_state` to reproduce the verification path above.

## What I learned

- How to keep an EXTI ISR short by publishing an event to main.
- How a 1 ms SysTick interrupt provides a non-blocking timebase.
- Why an elapsed-time state check must not depend on another button interrupt.
- How to ignore bounce events without restarting the first 50 ms interval.
- Why EXTI pending state records an event while `GPIOC_IDR` reports the current pin level.
- How `WFI` avoids busy-polling and how periodic SysTick interrupts affect wake frequency.
