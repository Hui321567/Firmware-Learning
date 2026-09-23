# STM32F401RE SPI1 Bare-Metal Loopback

## Overview

This milestone runs on the NUCLEO-F401RE and configures SPI1 through direct register access without STM32 HAL or LL APIs. SPI1 operates as a full-duplex master in Mode 0 (CPOL = 0, CPHA = 0), with 8-bit frames, MSB first, and a BR prescaler of /8. Transmission and reception use polling; no SPI interrupts or DMA are enabled.

The archived source preserves the final hardware-tested program: it sends one byte, `0x3C`, stores the received byte in `rx_data`, waits for the peripheral to become idle, releases software CS, and remains in an empty infinite loop.

## Hardware

| Function | MCU / board pin | Configuration |
|---|---|---|
| SPI1 SCK | PA5 / D13 | Alternate function AF5 |
| SPI1 MISO | PA6 / D12 | Alternate function AF5 |
| SPI1 MOSI | PA7 / D11 | Alternate function AF5 |
| Software CS | PA4 | GPIO output; low during the transaction |

Connect **D11 (MOSI / PA7) to D12 (MISO / PA6)** with a jumper for loopback. No external slave is used; PA4 CS does not need a loopback connection. SCK is generated on PA5/D13.

The program does not configure a PLL or change the APB2 prescaler. With the assumed reset clock configuration of PCLK2 = 16 MHz:

```text
SPI1_CR1.BR = 010 -> divide by 8
SCK = PCLK2 / 8 = 16 MHz / 8 = 2 MHz
```

2 MHz is calculated from the clock assumption and register configuration, not a measured waveform frequency.

## Control flow

```text
Enable GPIOA and SPI1 clocks -> PA4 output / CS high
       |
PA5/PA6/PA7 alternate function AF5 -> configure SPI1 -> SPE = 1
       |
CS low -> wait TXE = 1 -> write one byte to SPI1_DR
       |
MOSI transmits while MISO receives -> wait RXNE = 1 -> read SPI1_DR
       |
Wait BSY = 0 -> CS high -> empty infinite loop
```

## Register-level implementation

| Register | Purpose |
|---|---|
| `RCC_AHB1ENR` / `RCC_APB2ENR` | Enable GPIOA and SPI1 clocks |
| `GPIOA_MODER` / `GPIOA_AFRL` | PA4 output and PA5/PA6/PA7 AF5 routing |
| `GPIOA_BSRR` | Assert and release software CS |
| `SPI1_CR1` | Master, Mode 0, BR=/8, MSB first, 8-bit frame, software NSS, SPI enable |
| `SPI1_SR` | Poll TXE (bit 1), RXNE (bit 0), and BSY (bit 7) |
| `SPI1_DR` | Send and receive using volatile 8-bit register access |

`SSM = 1` and `SSI = 1` configure internal software NSS management; PA4 is controlled separately as the external CS signal. SPI is enabled after configuration. `spi1_transfer()` waits for TXE before writing and RXNE before reading. Main then waits for BSY to clear before raising CS. Other peripheral settings rely on reset defaults. Polling has no timeout or error recovery in this learning milestone.

## Debug verification

The user reported two successful physical-hardware loopback checks in STM32CubeIDE before archival:

| Transmitted byte | Debugger-observed received byte | Result |
|---|---|---|
| `0xA5` | `0xA5` | Passed |
| `0x3C` | `0x3C` | Passed |

The checks used the D11-to-D12 jumper and debugger observation of `tx_data` / `rx_data`, with TXE, RXNE, and BSY polling in the transfer sequence. The final source retains `tx_data = 0x3CU`. Debug was terminated after the completed checks. Archival does not repeat flashing or hardware validation.

## Project structure

```text
10-spi-baremetal/
|-- .project / .cproject       # STM32CubeIDE project metadata
|-- .settings/                # Portable Eclipse/CubeIDE settings
|-- Inc/                      # Reserved include directory (may be empty)
|-- Src/
|   |-- main.c                # SPI1 initialization and one-byte loopback
|   |-- syscalls.c
|   `-- sysmem.c
|-- Startup/
|   `-- startup_stm32f401retx.s
|-- STM32F401RETX_FLASH.ld
|-- STM32F401RETX_RAM.ld
`-- README.md
```

Source, startup, linker scripts, and portable project settings are preserved from `C:/STM32/workspace/10-spi-baremetal`. Generated Debug/Release directories, firmware images, local launch files, and workspace metadata are excluded from version control.

## Build and test

1. In STM32CubeIDE, select **File > Import > Existing Projects into Workspace**.
2. Select this directory. The project name is `10-spi-baremetal`; use a separate workspace if that name is already imported.
3. Build the Debug configuration. CubeIDE regenerates the build files.
4. Connect D11 to D12 and connect the NUCLEO-F401RE through ST-Link.
5. Start Debug and stop after the receive assignment to confirm `tx_data = rx_data = 0x3C`. Step past the BSY wait and `cs_high()` to complete the transaction.
6. To repeat the earlier pattern, change `tx_data` to `0xA5U`, rebuild and debug, and confirm `rx_data = 0xA5`. Restore `0x3CU` afterward to match the archived version.

### Archive rebuild verification

On 2026-09-23, STM32CubeIDE 2.2.0 imported the archival copy into a separate workspace and generated Debug build files. The headless IDE could not resolve the compiler path; running those generated files with GNU Make and an explicit GNU Tools for STM32 14.3.rel1 path completed compilation and linking with 0 errors and 2 warnings. The warnings are the existing template FPU-initialization diagnostic and `rx_data` being set but unused by the application (it is retained for debugger inspection). Application logic is integer-only. Source and original portable project settings are preserved. ELF size: text 1176 bytes, data 0 bytes, bss 1568 bytes (including linker-reserved heap/stack space). No hardware test was repeated during archival.

## What I learned

- SPI transfers data simultaneously on MOSI and MISO under the master's SCK.
- Alternate function AF5 routes SPI1 to PA5, PA6, and PA7.
- TXE permits a data write, RXNE permits a receive read, and BSY indicates ongoing peripheral activity.
- Software NSS management and an external GPIO CS are separate controls.
- Two loopback patterns verify the transmit/receive path; clock frequency requires separate timing measurement.
