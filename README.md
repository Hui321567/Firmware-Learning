# Firmware Learning Journey 🚀

Embedded Firmware Engineering practice repo focusing on Linux System Programming, MCU Drivers, Data Structures, and Core C Algorithms.

## 👤 About Me
- **Education**: National Taiwan Normal University - Mechatronics Engineering
- **Target**: Embedded Firmware / System Software Engineer

---

## 📁 Repository Structure

```text
Firmware-Learning/
├── 01-c-language/
│   └── bitwise-operations/   # Register bitwise manipulations (Set, Clear, Toggle, Check) with Makefile & Valgrind
├── 02-linux/
│   └── file-io/              # Linux System Programming (Custom my_cat implementation using syscalls)
├── 03-stm32/                 # [Coming Soon] Bare-metal & HAL drivers (GPIO, UART, Timer, Interrupt)
├── 04-data-structure/        # [Coming Soon] Firmware DS (Ring Buffer, Linked List)
├── 05-leetcode/              # Selected Firmware-focused LeetCode C solutions (Bit, Pointer, Memory)
└── 06-notes/                 # Hardware & OS concepts notes

## 🛠 Tech Stack & Environment

- **Language**: C (C99/C11) / Linux Shell (Bash)
- **Environment**: Ubuntu Linux / VS Code
- **Toolchain**: GCC, GDB, Make, Valgrind, Git

---

## 📌 Development Standard

Every project in this repository strictly adheres to:

1. Clear `README.md` explaining architecture and implementation logic.
2. Build automation using `Makefile`.
3. Memory leak verification with `Valgrind`.
4. Standard **Semantic Commit Messages** (`feat:`, `fix:`, `docs:`, `refactor:`).