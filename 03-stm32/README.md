# STM32 Bare-Metal & Driver Development

## Overview

Register-level firmware exercises for the NUCLEO-F401RE (STM32F401RE, Arm Cortex-M4). The GPIO milestones use direct memory-mapped registers rather than STM32 HAL or LL GPIO APIs.

## Projects

| Project | Status | Focus |
|---|---|---|
| `01-uart-driver/` | Completed (host simulation) | Register-level UART driver design |
| `02-gpio-baremetal/` | Completed on hardware | PA5 GPIO output and LED blink; standalone boot/link flow plus preserved CubeIDE project |
| `03-gpio-button-polling/` | Completed on hardware | PC13 active-low polling, state-change detection, PA5 LED toggle, and software debounce |

## Hardware and tools

- Board: ST NUCLEO-F401RE
- MCU: STM32F401RE
- IDE: STM32CubeIDE
- Standalone toolchain: `arm-none-eabi-gcc`, GNU Make, OpenOCD or ST-Link tools

Each project has its own README with implementation, build workflow, and verification notes.

## Next steps

- Replace blocking debounce with timer-based, non-blocking input handling.
- Implement EXTI button interrupts.
- Port UART to hardware and combine interrupt-driven RX with the ring buffer module.
