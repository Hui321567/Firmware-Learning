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

**Milestone 1: Initialization and Page Read**

Implemented and tested:
- NAND initialization
- Page Read
- Basic error handling

Planned:
- NAND Program
- Block Erase
- NAND operation constraints
- Mini-FTL (Flash Translation Layer)

## Development Environment

- Language: C11
- OS: Ubuntu Linux
- Compiler: GCC
- Build Tool: GNU Make