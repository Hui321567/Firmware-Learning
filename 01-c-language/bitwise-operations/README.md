# C Bitwise Operations & Register Control

## 1. Overview
This project demonstrates fundamental bitwise operations commonly used in embedded firmware, bitmasking, and hardware register control.

## 2. Learning Objectives
- Understand bitwise operators in C (`&`, `|`, `^`, `~`, `<<`, `>>`).
- Practice manipulating individual bits without affecting other register bits.
- Master register-style bit manipulation idiom commonly seen in MCU drivers.

## 3. Key Concepts
- **Set Bit**: Setting a bit to 1 using bitwise OR (`|=`).
- **Clear Bit**: Clearing a bit to 0 using bitwise AND with inverted mask (`&= ~`).
- **Check Bit**: Testing a bit status using bitwise AND (`&`).
- **Toggle Bit**: Inverting a bit using bitwise XOR (`^=`).
- **Bit Mask**: Constructing masks dynamically with shift operators (`(1U << bit)`).

## 4. Implementation
The project uses an 8-bit variable to simulate a hardware control register. 

Operations demonstrated in `bitwise_demo.c`:
* Setting Bit 3 (`reg |= (1U << 3)`)
* Clearing Bit 3 (`reg &= ~(1U << 3)`)
* Toggling Bit 5 (`reg ^= (1U << 5)`)
* Checking Bit status

## 5. Project Structure
```text
.
├── bitwise_demo.c
├── Makefile
└── README.md

6. Build & Run

Bash
# Compile
make

# Run demonstration
./bitwise_demo

# Clean build artifacts
make clean

7. Verification
Verified that register bit values match expected binary and hexadecimal representations in the terminal output.

8. What I Learned
How bitwise masks prevent unwanted modifications to adjacent register fields.

Why unsigned integer shifts (1U << n) are critical to avoid undefined behavior in C.