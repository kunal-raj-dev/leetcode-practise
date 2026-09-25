// ============================================================
// Problem  : Two Sum
// Approach : Hash Map (One Pass)
// Time     : O(N)   — single loop through the array
// Space    : O(N)   — hash map stores up to N elements
// ============================================================
// THE CORE INSIGHT:
//   For each nums[i], we need (target - nums[i]).
//   Instead of scanning the array for it (O(N)),
//   we store seen numbers in a hash map → look up in O(1).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// unordered_map<int, int>:
//   KEY   = the number (nums[i])
//   VALUE = its index (i)
// This lets us answer "have I seen number X before? if yes, at what index?"

vector<int> twoSum(vector<int>& nums, int target) {
    // mp stores: { number_seen → its_index }
    unordered_map<int, int> mp;

    for (int i = 0; i < (int)nums.size(); i++) {

        int complement = target - nums[i];
        // WHY "complement"? Because nums[i] + complement = target
        // So if complement exists in array → we found our pair

        // Check if we've already seen the complement
        if (mp.count(complement)) {
            // mp.count(key) returns 1 if key exists, 0 if not
            // If complement exists → return both indices
            return {mp[complement], i};
            // mp[complement] = index where complement was seen
            // i             = current index
        }

        // Complement not found yet → store current number with its index
        mp[nums[i]] = i;
        // WHY store AFTER checking? Edge case safety:
        // If nums[i] == complement (e.g., nums=[3,3], target=6)
        // We don't want to match nums[i] with itself.
        // Storing after checking prevents self-matching.
    }

    return {}; // guaranteed never reached (problem says solution always exists)
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // --- Test Case 1 ---
    vector<int> nums1 = {2, 7, 11, 15};
    int target1 = 9;
    vector<int> ans1 = twoSum(nums1, target1);
    cout << "Test 1: [" << ans1[0] << ", " << ans1[1] << "]" << endl;
    // Expected: [0, 1]

    // --- Test Case 2 ---
    vector<int> nums2 = {3, 2, 4};
    int target2 = 6;
    vector<int> ans2 = twoSum(nums2, target2);
    cout << "Test 2: [" << ans2[0] << ", " << ans2[1] << "]" << endl;
    // Expected: [1, 2]

    // --- Test Case 3: Same values at different indices ---
    vector<int> nums3 = {3, 3};
    int target3 = 6;
    vector<int> ans3 = twoSum(nums3, target3);
    cout << "Test 3: [" << ans3[0] << ", " << ans3[1] << "]" << endl;
    // Expected: [0, 1]

    // --- Test Case 4: Negative numbers ---
    vector<int> nums4 = {-1, -2, -3, -4, -5};
    int target4 = -8;
    vector<int> ans4 = twoSum(nums4, target4);
    cout << "Test 4: [" << ans4[0] << ", " << ans4[1] << "]" << endl;
    // Expected: [2, 4]  (-3 + -5 = -8)

    return 0;
}

// ============================================================
// TRACE THROUGH (read this carefully — this is how to debug):
//
// nums = [2, 7, 11, 15], target = 9
//
// i=0: num=2, complement=9-2=7
//      Is 7 in map? NO (map is empty: {})
//      Store 2: map = {2:0}
//
// i=1: num=7, complement=9-7=2
//      Is 2 in map? YES! at index 0
//      Return {map[2], 1} = {0, 1} ✅
//
// ─────────────────────────────────────────────────────────
// nums = [3, 3], target = 6  (same-value edge case)
//
// i=0: num=3, complement=6-3=3
//      Is 3 in map? NO (map is empty: {})
//      Store 3: map = {3:0}
//
// i=1: num=3, complement=6-3=3
//      Is 3 in map? YES! at index 0
//      Return {map[3], 1} = {0, 1} ✅
//      (Different indices — correct! Not using same element twice)
// ============================================================
