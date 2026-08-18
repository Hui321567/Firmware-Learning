# C Memory Layout & Struct Alignment Architecture

## 1. C Process Memory Layout
A typical C program execution memory image is divided into five main segments:

```text
+-----------------------------------+  High Address (0xFFFF...)
|         Kernel Space              |  (User space process cannot access)
+-----------------------------------+
|         Stack Segment             |  <-- Grows Downward
|  - Local variables                |
|  - Function call frames / args    |
|               │                   |
|               ▼                   |
|               ▲                   |
|               │                   |
|  - Dynamic allocations (malloc)   |
|         Heap Segment              |  <-- Grows Upward
+-----------------------------------+
|  BSS Segment                      |  Uninitialized global & static variables
|  - Initialized to 0 by runtime    |  (Occupies no space in ELF binary)
+-----------------------------------+
|  Data Segment                     |  Initialized global & static variables
|  - e.g., int global_var = 10;     |  (Stored in binary file)
+-----------------------------------+
|  Text (Code) Segment              |  Executable machine instructions & 
|  - Read-only constants            |  string literals
+-----------------------------------+  Low Address (0x0000...)
```

## 2. Memory Alignment & Struct Padding
Why Alignment?
Modern CPUs read memory in words (e.g., 4 bytes on 32-bit CPU, 8 bytes on 64-bit CPU). Aligning data on word boundaries minimizes memory bus cycles needed to access a variable.

Case Study: Default vs. Packed Structs
Default Struct (With Alignment Padding)
```C
struct DefaultStruct {
    uint8_t  a;  // 1 byte
    uint32_t b;  // 4 bytes
    uint8_t  c;  // 1 byte
};
```
Memory Representation (12 Bytes Total):

```Plaintext
Byte Offset:  0      1      2      3      4      5      6      7
            +------+------+------+------+------+------+------+------+
            |  a   |    Pad (3B)        |          b (Byte 0~1)     |
            +------+------+------+------+------+------+------+------+
Byte Offset:  8      9      10     11
            +------+------+------+------+
            |          b (Byte 2~3)     |  c   |    Pad (3B)        |
            +------+------+------+------+------+------+------+------+
```
Packed Struct (__attribute__((packed)))
```C
struct __attribute__((packed)) PackedStruct {
    uint8_t  a;  // 1 byte
    uint32_t b;  // 4 bytes
    uint8_t  c;  // 1 byte
};
```
Memory Representation (6 Bytes Total):

```Plaintext
Byte Offset:  0      1      2      3      4      5
            +------+------+------+------+------+------+
            |  a   |          b (4 Bytes)      |  c   |
            +------+------+------+------+------+------+
```
## 3. Firmware Applications
- 1.Packed Structs: Essential when parsing hardware network protocol headers (TCP/IP, CAN bus) or memory-mapped hardware control registers where padding byte offsets would ruin data layout alignment.

- 2.Aligned Allocation: Critical for DMA (Direct Memory Access) buffers, where MCU DMA hardware controllers mandate buffer address alignment on 32-byte or 64-byte boundary.
