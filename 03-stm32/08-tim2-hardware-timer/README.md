# STM32F401RE TIM2 Hardware Timer + Update Interrupt

## Overview

This milestone runs on the NUCLEO-F401RE and uses direct register access without STM32 HAL or LL APIs. TIM2 generates an update interrupt approximately once per second. The interrupt handler clears the update flag and toggles LD2 on PA5 through GPIOA_BSRR.

Main initializes GPIO and TIM2, then stays in an empty infinite loop. Timing comes from the hardware timer; this version does not use WFI or a software delay.

## Hardware

| Function | MCU pin | Electrical behavior |
|---|---|---|
| LD2 LED | PA5 | High turns the LED on |

The project relies on the reset clock configuration with a 16 MHz TIM2 input clock. It does not configure a PLL or change the APB1 prescaler.

```text
16,000,000 / (15,999 + 1) = 1,000 Hz counter clock
1,000 / (999 + 1) = 1 Hz update interrupt
```

Each counter tick is 1 ms. LD2 changes state approximately every second; a complete on/off cycle takes approximately two seconds.

## Control flow

```text
Enable GPIOA clock -> configure PA5 output -> LED initially off
       |
Enable TIM2 clock -> PSC = 15999 -> ARR = 999
       |
Generate UG to load prescaler -> clear initialization UIF
       |
Enable update interrupt (UIE) -> enable NVIC IRQ28 -> set CEN
       |
Counter overflow -> TIM2_IRQHandler
       |
Check UIF -> clear UIF -> toggle LED state through GPIOA_BSRR
```

## Register-level implementation

| Register | Purpose |
|---|---|
| `RCC_AHB1ENR` / `RCC_APB1ENR` | Enable GPIOA and TIM2 clocks |
| `GPIOA_MODER` / `GPIOA_BSRR` | Configure and control PA5 |
| `TIM2_PSC` / `TIM2_ARR` | Divide the timer clock and set the update interval |
| `TIM2_EGR` | Generate UG to apply the initial prescaler |
| `TIM2_SR` | Test and clear UIF |
| `TIM2_DIER` | Enable update interrupts using UIE |
| `NVIC_ISER0` | Enable TIM2 IRQ28 using bit 28 |
| `TIM2_CR1` | Start counting using CEN |

The startup vector table points to `TIM2_IRQHandler`; the definition in `Src/main.c` overrides the weak default handler. The ISR-local static `led_state` retains the LED state between interrupts.

## Debug verification

The following physical-hardware results were reported from the completed source project before archival:

1. STM32CubeIDE Build succeeded.
2. A breakpoint in `TIM2_IRQHandler` was reached.
3. UIF was 1 on ISR entry and 0 after clearing it.
4. LD2 on PA5 toggled approximately once per second.
5. Debug register inspection confirmed `TIM2_PSC = 15999`, `TIM2_ARR = 999`, and `TIM2_CR1.CEN = 1`.

UIF can already be zero when pausing in main because the ISR has acknowledged the interrupt. These hardware observations are distinct from the archive rebuild check; archival does not repeat flashing or physical timing measurements.

## Project structure

```text
08-tim2-hardware-timer/
|-- .project / .cproject       # STM32CubeIDE project metadata
|-- .settings/                # Portable Eclipse/CubeIDE settings
|-- Inc/                      # Reserved include directory
|-- Src/
|   |-- main.c                # GPIO, TIM2 initialization and update ISR
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f401retx.s
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
`-- README.md
```

Generated Debug/Release directories, objects, firmware images, workspace metadata, local launch files, and register-view preferences are excluded by the repository `.gitignore`.

## Build and test

1. Open STM32CubeIDE and select **File > Import > Existing Projects into Workspace**.
2. Select this directory. The project name is `08-tim2-hardware-timer`. Use a separate workspace if the original project with the same name is already imported.
3. Build the Debug configuration. CubeIDE regenerates build files from the archived project settings.
4. Connect the NUCLEO-F401RE through ST-Link and run or debug the firmware.
5. Confirm LD2 changes state approximately once per second.
6. Optionally break in `TIM2_IRQHandler` and reproduce the UIF and register checks above; resume execution for visual timing checks.

### Archive rebuild verification

On 2026-09-15, the archived project was imported into a separate workspace and rebuilt from source using STM32CubeIDE 2.2.0 and its bundled GNU Tools for STM32 14.3.rel1. Debug completed with 0 errors and 1 warning. The warning is the existing main.c template diagnostic about compiling for an FPU without initializing it. This milestone uses integer-only application logic; the original source and build settings are preserved. ELF size: text 968 bytes, data 0 bytes, bss 1568 bytes (including linker-reserved heap/stack space).

## What I learned

- How PSC and ARR combine to set a hardware timer interval.
- Why generating UG requires clearing the initial update flag before enabling interrupts.
- How peripheral interrupt enable, NVIC enable, and counter enable work together.
- How to connect a C interrupt handler to the startup vector table.
- How to validate timer behavior using both debugger registers and a physical LED.
