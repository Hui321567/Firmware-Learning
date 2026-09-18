# STM32F401RE TIM2 Hardware PWM

## Overview

This milestone runs on the NUCLEO-F401RE and uses direct register access without STM32 HAL or LL APIs. TIM2_CH1 drives PA5 through alternate function AF1 to control LD2 with hardware PWM. The final configuration is nominally 1 kHz at 50% duty cycle.

Main initializes GPIO and TIM2, then stays in an empty infinite loop. PWM generation requires no application ISR, NVIC configuration, software delay, or repeated CPU pin toggling.

## Hardware

| Function | MCU pin | Configuration |
|---|---|---|
| LD2 LED | PA5 | AF1 / TIM2_CH1; active high |

The project relies on the reset clock configuration with a 16 MHz TIM2 input clock. It does not configure a PLL or change the APB1 prescaler. Timer output mode, up-counting, and active-high polarity rely on the peripheral reset defaults.

```text
Counter clock = 16,000,000 / (PSC + 1)
              = 16,000,000 / (15 + 1) = 1,000,000 Hz
PWM frequency = 1,000,000 / (ARR + 1)
              = 1,000,000 / (999 + 1) = 1,000 Hz
Duty cycle    = CCR1 / (ARR + 1) = 500 / 1000 = 50%
```

Each counter tick is 1 us; the nominal PWM period is 1 ms. In PWM mode 1 with up-counting and active-high polarity, output is high while CNT < CCR1.

## Control flow

```text
Enable GPIOA clock -> PA5 MODER = 10 (alternate function) -> AFRL AF1
       |
Enable TIM2 clock -> PSC = 15 -> ARR = 999 -> CCR1 = 500
       |
OC1M = 110 (PWM mode 1) -> OC1PE = 1 -> CC1E = 1 -> ARPE = 1
       |
Write EGR.UG to load initial values -> CR1.CEN = 1
       |
TIM2_CH1 -> PA5 -> LD2; main remains in an empty loop
```

## Register-level implementation

| Register | Purpose |
|---|---|
| `RCC_AHB1ENR` / `RCC_APB1ENR` | Enable GPIOA and TIM2 clocks |
| `GPIOA_MODER` / `GPIOA_AFRL` | Route PA5 to AF1 / TIM2_CH1 |
| `TIM2_PSC` / `TIM2_ARR` | Set counter clock and PWM period |
| `TIM2_CCR1` | Set channel 1 duty cycle; final value 500 |
| `TIM2_CCMR1` | Select PWM mode 1 and enable CCR1 preload |
| `TIM2_CCER` | Enable channel 1 output |
| `TIM2_CR1` | Enable ARR preload and start counting |
| `TIM2_EGR` | Generate UG using direct assignment |

The final source uses `PA5_PIN` and `TIM2_EGR = (1U << 0U)`. CCR1 and ARR preload synchronize changes with update events. Update interrupts are not enabled; no application TIM2 interrupt handler is used.

## Debug verification

The user reported the following physical-hardware results from the completed source project before archival:

1. Final STM32CubeIDE Build succeeded.
2. Debug -> Resume/F8 ran successfully on the NUCLEO-F401RE.
3. Changing CCR1 produced the following LD2 brightness progression:

| CCR1 | Calculated duty cycle | Reported LD2 observation |
|---|---|---|
| 250 | 25% | Dimmer |
| 500 | 50% | Intermediate brightness |
| 750 | 75% | Brighter |

4. CCR1 was restored to 500 for the final Build and hardware check.

Frequency and duty values above are calculated from the configured 16 MHz clock and register values. Visual brightness checks confirm the reported progression; no oscilloscope or logic-analyzer timing measurement was supplied. Archive rebuilding is separate from these user-reported hardware checks.

## Project structure

```text
09-tim2-pwm/
|-- .project / .cproject       # STM32CubeIDE project metadata
|-- .settings/                # Portable Eclipse/CubeIDE settings
|-- Inc/                      # Reserved include directory (may be empty)
|-- Src/
|   |-- main.c                # PA5 AF1 and TIM2 hardware PWM initialization
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f401retx.s
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
`-- README.md
```

Source, startup, linker scripts, and portable project settings are preserved from `C:/STM32/workspace/09-tim2-pwm`. Generated Debug/Release directories, firmware images, local launch files, and workspace metadata are excluded from version control.

## Build and test

1. In STM32CubeIDE, select **File > Import > Existing Projects into Workspace**.
2. Select this directory. The project name is `09-tim2-pwm`; use a separate workspace if that name is already imported.
3. Build the Debug configuration. CubeIDE regenerates the build files.
4. Connect the NUCLEO-F401RE through ST-Link, start Debug, and select Resume/F8.
5. Confirm LD2 is lit with the final 50% setting. Optionally repeat CCR1 = 250/500/750 builds and compare brightness, restoring 500 afterward.

### Archive rebuild verification

On 2026-09-18, the archival copy was imported into a separate workspace and rebuilt with STM32CubeIDE 2.2.0. Debug completed with 0 errors and 1 warning: the existing template diagnostic about compiling for an FPU without initializing it. Application logic is integer-only; the original source and build settings are preserved. ELF size: text 924 bytes, data 0 bytes, bss 1568 bytes (including linker-reserved heap/stack space). Archival did not repeat flashing or hardware measurements.

## What I learned

- PSC and ARR determine PWM frequency; CCR1 determines duty cycle.
- Alternate-function routing gives TIM2_CH1 control of PA5.
- Hardware PWM continues without an ISR or CPU-driven GPIO toggling.
- Preload and an initial UG apply timer settings predictably.
- LED brightness observations and measured waveform timing provide different levels of verification.
