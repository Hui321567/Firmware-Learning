/*
 * LeetCode 231. Power of Two
 * Key Technique: n & (n - 1) == 0
 * Time Complexity: O(1)
 */
#include <stdbool.h>

bool isPowerOfTwo(int n) {
    // A power of two must be positive and has only one bit set in binary
    return (n > 0) && ((n & (n - 1)) == 0);
}