
# NAND Flash Simulator

## Overview

A NAND Flash simulator implemented in C for learning NAND Flash architecture, memory operations, and SSD firmware fundamentals.

This project simulates NAND Flash using memory arrays. No physical NAND hardware is required.

The project also includes a simplified Flash Translation Layer (FTL) for logical-to-physical address mapping and out-of-place updates.

## NAND Geometry

| Parameter | Value |
|---|---|
| Page Size | 256 Bytes |
| Pages per Block | 4 |
| Block Count | 2 |
| Total Pages | 8 |
| Total Capacity | 2048 Bytes |
| Logical Pages | 4 |

## Implemented Features

### NAND Flash Layer

- NAND Flash initialization (erased state: 0xFF)
- Physical Page Number (PPN) to Block/Page conversion
- Page Read API (`nand_read`)
- Page Program API (`nand_program`)
- Block Erase API (`nand_erase`)
- Invalid PPN detection
- NULL buffer detection
- Full-page (256-byte) read verification
- 1-to-0 bit transition validation
- Invalid Program rejection (return -3)
- Failed Program preserves existing page data
- Block index boundary validation
- Block-level erase and data isolation

### Mini-FTL Layer

- Logical-to-Physical (L2P) Mapping Table
- Invalid PPN representation (`INVALID_PPN`)
- FTL initialization (`ftl_init`)
- FTL Write API (`ftl_write`)
- FTL Read API (`ftl_read`)
- Sequential Physical Page allocation
- Out-of-place Update
- Preservation of old physical page data
- Logical mapping updates after successful NAND programming

### Testing

- Nine NAND / FTL functional tests
- Independent test functions
- PASS / FAIL reporting
- Test summary and process exit status
- Full-page data verification using `memcmp`

## Build and Run

Build the simulator:

```bash
make
```

Run the tests:

```bash
./nand_sim
```

Clean build output:

```bash
make clean
```

## Current Status

**Milestone 4: Mini-FTL L2P Mapping and Out-of-place Update**

Completed:
- NAND Initialization
- NAND Page Read / Program / Erase
- Bit transition validation
- Failed Program data preservation
- Block-level erase and data isolation
- L2P Mapping Table
- FTL Read / Write
- Sequential Physical Page allocation
- Out-of-place Update
- Test refactoring (9 tests passed)

## Planned Features

- Physical Page State Management (FREE / VALID / INVALID)
- Invalid Page tracking
- Free Page management
- Garbage Collection
- Wear Leveling
- Additional NAND operation restrictions
- Expanded FTL tests

## Limitations

This project is a simplified NAND Flash and FTL simulator.

- NAND storage is simulated in RAM.
- NAND data and mapping information are not persistent.
- Physical Page allocation is sequential.
- Garbage Collection and Wear Leveling are not yet implemented.
- Some physical NAND programming restrictions are not modeled.
