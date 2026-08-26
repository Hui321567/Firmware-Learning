# STM32F401RE Bare-Metal GPIO LED Blink

## 1. Overview
This project implements a bare-metal LED blink firmware for the STMicroelectronics NUCLEO-F401RE development board. Developed entirely from scratch without vendor-provided HAL or SPL libraries, it demonstrates the full execution lifecycle of an ARM Cortex-M4 microcontroller—from power-on reset, startup vector execution, and custom C Runtime (CRT) memory initialization, to direct memory-mapped register manipulation for GPIO output control.

## 2. Learning Objectives
* Understand the ARM Cortex-M4 hardware boot sequence and initial vector table fetching mechanism.
* Write a custom startup file (`Reset_Handler`) to manually handle C Runtime (CRT) setup (`.data` copying and `.bss` clearing).
* Craft a GNU Linker Script (`stm32f401.ld`) to define physical FLASH and SRAM memory regions and section layouts.
* Configure peripheral clock gates (RCC) and GPIO control registers using C structure pointer dereferencing and bitwise operations.
* Master the automated build flow using `arm-none-eabi-gcc`, `arm-none-eabi-objcopy`, and GNU Make.

## 3. Key Concepts
* **Vector Table & Boot Vector**: Memory-mapped table starting at `0x08000000` containing the Initial Main Stack Pointer (`_estack`) and `Reset_Handler` address.
* **Dual-Address Memory Sections (VMA vs. LMA)**: Loading initialized global variables (`.data`) into FLASH at build time (LMA) while executing them inside SRAM at runtime (VMA).
* **Linker Symbols**: Referencing symbols exported by the linker script (`_sidata`, `_sdata`, `_edata`, `_sbss`, `_ebss`) as memory boundary pointers in C.
* **Memory-Mapped I/O (MMIO)**: Accessing RCC and GPIO peripheral registers via volatile-qualified structure pointers mapped to hardware addresses (`0x40023800`, `0x40020000`).

## 4. Environment Setup
* **Cross Compiler**: `arm-none-eabi-gcc` (GNU Toolchain for ARM)
* **Build System**: GNU `make`
* **Flashing Utility**: `st-flash` (`stlink-tools`) or OpenOCD
* **Hardware Target**: STMicroelectronics NUCLEO-F401RE (STM32F401RE MCU, 512 KB Flash, 96 KB SRAM)

## 5. Project Structure
```text
02-gpio-baremetal/
├── main.c          # Vector table, Reset_Handler (CRT setup), and GPIO register control logic
├── stm32f401.ld    # GNU Linker Script specifying FLASH/SRAM boundaries and section placement
├── Makefile        # Build rules for compilation, linking, size analysis, and binary generation
└── README.md       # Project documentation
```
## 6. Build & Flash Workflows
Build Firmware
Compile C source code, link with custom linker script, and output binary payload:

```Bash
make
```
- Outputs generated in build/ directory:

- build/firmware.elf: Executable with full debug symbols and section header tables.

- build/firmware.bin: Strip binary payload ready for Flash programming.

- build/firmware.map: Linker-generated memory map report.

Clean Artifacts
```Bash
make clean
```
Flash Firmware via ST-Link
Connect the NUCLEO board via USB and flash the binary to Flash base address (0x08000000):

```Bash
st-flash write build/firmware.bin 0x08000000
```
## 7. Verification / Debugging
Compiler Output & Memory Footprint Verification
Check firmware section allocation sizes output by arm-none-eabi-size:

```Plaintext
arm-none-eabi-size build/firmware.elf
   text    data     bss     dec     hex filename
    200       8       0     208      d0 build/firmware.elf
```
- text (200 bytes): Vector table, Reset_Handler, and main() instructions placed in Flash.

- data (8 bytes): Initialized global data backup stored in Flash and copied to SRAM at boot.

- bss (0 bytes): Uninitialized data section in SRAM.

Hardware Validation
Press the black RESET button on NUCLEO-F401RE. The onboard green LD2 LED (PA5) toggles periodically, validating correct vector table execution, CRT memory initialization, clock gate configuration, and GPIO output toggling.

## 8. What I Learned
- main() is Not the First Code Executed: In ARM Cortex-M bare-metal, hardware directly reads index 0 (MSP) and index 1 (Reset_Handler) from the vector table at 0x08000000.

- C Runtime (CRT) Handshake: Uninitialized global variables (.bss) must be zero-cleared and initialized variables (.data) must be copied from Flash (LMA) to SRAM (VMA) before entering main().

- Linker Script Location Counter (.): How location counter movement and alignment (ALIGN(4)) inside section definitions dynamically compute memory markers.

- Volatile Register Access: Why register maps must use volatile type qualifiers to prevent compiler optimization from stripping repeated hardware register accesses in delay loops.