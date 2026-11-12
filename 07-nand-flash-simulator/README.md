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
- Block Erase API (`nand_erase`)
- Block index boundary validation
- Block-level erase and data isolation
- Seven basic PASS/FAIL tests
- Test summary and process exit status

## Build and Run

```bash
make
./nand_sim
```

Clean build output:

```bash
make clean
```

### Current Status

**Milestone 3: NAND Block Erase**

Implemented and tested:
- NAND Initialization
- NAND Page Read
- NAND Page Program
- NAND Block Erase
- Bit transition validation
- Failed Program data preservation
- Basic block isolation
- Seven automated checks with PASS/FAIL reporting

Planned:
- Additional NAND operation restrictions
- Expanded unit tests
- Mini-FTL (Flash Translation Layer)