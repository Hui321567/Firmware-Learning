/*
 * LeetCode 136. Single Number
 * Key Technique: XOR Operation (a ^ a = 0, a ^ 0 = a)
 * Time Complexity: O(N), Space Complexity: O(1)
 */
int singleNumber(int* nums, int numsSize) {
    int result = 0;
    for (int i = 0; i < numsSize; i++) {
        result ^= nums[i]; // XOR operation cancels out duplicate numbers
    }
    return result;
}