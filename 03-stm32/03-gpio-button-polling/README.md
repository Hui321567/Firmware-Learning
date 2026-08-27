# STM32F401RE Bare-Metal GPIO Button Polling

## Overview

This milestone reads the NUCLEO-F401RE USER button directly from the GPIO input register and toggles the onboard LD2 LED after a debounced press. It uses no STM32 HAL or LL GPIO APIs; all peripheral configuration and I/O use direct memory-mapped register access.

The final firmware was validated on physical NUCLEO-F401RE hardware.

## Hardware

| Function | MCU pin | Electrical behavior |
|---|---|---|
| LD2 LED | PA5 | High turns the LED on |
| USER button B1 | PC13 | Active-low: pressed reads `0` |

## Register-level implementation

- `RCC_AHB1ENR` enables the GPIOA and GPIOC peripheral clocks.
- `GPIOA_MODER` configures PA5 as a general-purpose output.
- `GPIOC_MODER` configures PC13 as an input.
- `GPIOC_IDR` samples the USER button.
- `GPIOA_BSRR` atomically sets or resets the LED output.
- A state-change check handles both press and release transitions.
- Only a stable released-to-pressed transition toggles the LED.
- `DEBOUNCE_DELAY_COUNT` is `50000U` for the software debounce delay.

## Behavior

```text
released -> pressed -> debounce -> LED toggles
pressed  -> released -> debounce -> state updates only
```

Updating the saved state on release is essential. Otherwise, the next button press would not be recognized as a new edge.

## Project structure

```text
03-gpio-button-polling/
|-- .project / .cproject       # STM32CubeIDE project metadata
|-- .settings/                 # Portable Eclipse/CubeIDE settings
|-- Src/
|   |-- main.c                 # GPIO, polling, edge detection and debounce
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f401retx.s
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
`-- README.md
```

Generated `Debug/` and `Release/` directories, workspace `.metadata/`, and machine-local launch files are intentionally excluded.

## Import and run

1. In STM32CubeIDE, select **File > Import > Existing Projects into Workspace**.
2. Select this directory as the project root.
3. Build the Debug configuration.
4. Flash and run on a NUCLEO-F401RE.
5. Press and release B1 repeatedly. LD2 should toggle once per confirmed press and remain unchanged on release.

## What I learned

- How active-low inputs change the meaning of a raw GPIO bit.
- Why edge/state-change detection must accept both press and release states.
- How a second sample after a delay filters mechanical contact bounce.
- Why `BSRR` is preferable to a read-modify-write operation for setting and resetting GPIO outputs.
