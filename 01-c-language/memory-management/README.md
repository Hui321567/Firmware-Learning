# C Struct Memory Alignment & Padding

Demonstration of C memory alignment, structure padding rules, and GCC `__attribute__((packed))` behavior.

## Key Takeaways

1. **Member Alignment**: Member offset must be a multiple of its own type size.
2. **Structure Alignment**: Total size of a struct must be a multiple of its largest member's size.
3. **Packed Attribute**: Using `__attribute__((packed))` strips all padding bytes, which is useful for register mapping and network packet headers, but comes with performance penalty on unaligned memory access.

## Build & Run

```bash
make
./padding_test