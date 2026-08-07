/*
 * LeetCode 191. Number of 1 Bits
 * Key Technique: Brian Kernighan's Algorithm (n & (n - 1))
 * Time Complexity: O(K), where K is the number of 1s (Faster than O(32))
 */
#include <stdint.h>

int hammingWeight(uint32_t n) {
    int count = 0;
    while (n != 0) {
        n &= (n - 1); // Clears the lowest set bit
        count++;
    }
    return count;
}