# NAND Flash Simulator

## Overview

A NAND Flash simulator implemented in C for learning NAND Flash architecture, memory operations, and SSD firmware fundamentals.

This project simulates NAND Flash using memory arrays. No physical NAND hardware is required.

## NAND Geometry

| Parameter | Value |
|---|---|
| Page Size | 256 Bytes |
| Pages per Block | 4 |
| Block Count | 2 |
| Total Pages | 8 |
| Total Capacity | 2048 Bytes |

## Implemented Features

- NAND Flash initialization (erased state: 0xFF)
- Physical Page Number (PPN) to Block/Page conversion
- Page Read API (`nand_read`)
- Invalid PPN detection
- NULL buffer detection
- Full-page (256-byte) read verification
- Page Program API (`nand_program`)
- 1-to-0 bit transition validation
- Invalid Program rejection (return -3)
- Failed Program preserves existing page data

## Build and Run

```bash
make
./nand_sim
```

Clean build output:

```bash
make clean
```

## Current Status

**Milestone 2: NAND Page Program**

Implemented and tested:
- NAND Initialization
- NAND Page Read
- NAND Page Program
- Invalid PPN and NULL buffer handling
- Invalid bit transition detection
- Failed Program data preservation

Planned:
- NAND Block Erase
- Page Program restrictions
- Additional unit tests
- Mini-FTL (Flash Translation Layer)

## Development Environment

- Language: C11
- OS: Ubuntu Linux
- Compiler: GCC
- Build Tool: GNU Make