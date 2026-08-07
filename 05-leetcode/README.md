# LeetCode Firmware Practice Notes

- **Language**: C
- **Focus**: Bitwise Operations, Pointers, Linked Lists, Memory Management

## Selected Solutions

### Bit Manipulation
| Problem | Solution | Key Concept / Trick |
| :--- | :--- | :--- |
| [191. Number of 1 Bits](./bit/lc191_number_of_1_bits.c) | `n & (n - 1)` | Brian Kernighan's Algorithm to clear lowest set bit |
| [136. Single Number](./bit/lc136_single_number.c) | XOR `^` | `a ^ a = 0`, `a ^ 0 = a` property |
| [231. Power of Two](./bit/lc231_power_of_two.c) | `n & (n - 1) == 0` | Power of two has exactly one bit set |