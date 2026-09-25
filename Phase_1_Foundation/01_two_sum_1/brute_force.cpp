// ============================================================
// Problem  : Two Sum
// Approach : Brute Force — Check Every Pair
// Time     : O(N^2) — two nested loops
// Space    : O(1)   — no extra memory used
// ============================================================
// WHEN TO USE THIS: Always write this first to confirm you
// understood the problem. Never skip the brute force.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// NOTE: ios_base::sync_with_stdio(false) and cin.tie(NULL)
// are fast I/O lines. They disconnect C and C++ I/O streams
// making cin/cout as fast as scanf/printf. Always include
// in competitive programming. Safe to use in interviews too.

vector<int> twoSum(vector<int>& nums, int target) {
    int n = nums.size(); // total number of elements

    // Outer loop: pick the first element of the pair
    for (int i = 0; i < n; i++) {

        // Inner loop: pick the second element (always starts at i+1)
        // WHY i+1? Because:
        //   1. We don't want to use the same element twice (i != j)
        //   2. Pairs (i,j) and (j,i) are the same — no need to check both
        for (int j = i + 1; j < n; j++) {

            // Check if this pair sums to target
            if (nums[i] + nums[j] == target) {
                return {i, j}; // found the answer — return both indices
            }
        }
    }

    // The problem guarantees exactly one solution exists,
    // so we will always return before reaching here.
    // This line just keeps the compiler happy.
    return {};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // --- Test Case 1 ---
    vector<int> nums1 = {2, 7, 11, 15};
    int target1 = 9;
    vector<int> ans1 = twoSum(nums1, target1);
    cout << "Test 1: [" << ans1[0] << ", " << ans1[1] << "]" << endl;
    // Expected: [0, 1]  (2 + 7 = 9)

    // --- Test Case 2 ---
    vector<int> nums2 = {3, 2, 4};
    int target2 = 6;
    vector<int> ans2 = twoSum(nums2, target2);
    cout << "Test 2: [" << ans2[0] << ", " << ans2[1] << "]" << endl;
    // Expected: [1, 2]  (2 + 4 = 6)

    // --- Test Case 3: Same values at different indices ---
    vector<int> nums3 = {3, 3};
    int target3 = 6;
    vector<int> ans3 = twoSum(nums3, target3);
    cout << "Test 3: [" << ans3[0] << ", " << ans3[1] << "]" << endl;
    // Expected: [0, 1]  (3 + 3 = 6)

    return 0;
}

// ============================================================
// TRACE THROUGH TEST 1 (learn to mentally trace code):
//
// nums = [2, 7, 11, 15], target = 9
//
// i=0 (nums[0]=2):
//   j=1 (nums[1]=7):  2+7=9 == 9 ✅ → return {0, 1}
//
// Never even reaches i=1. Done.
// ============================================================
