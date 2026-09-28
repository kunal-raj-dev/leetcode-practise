// ============================================================
// Approach 3 — STL insert()
// Time     : O(N)
// Space    : O(N)
// ============================================================

#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        vector<int> ans = nums;

        ans.insert(ans.end(), ans.begin(), ans.end());
        // ans.insert(ans.end(), nums.begin(), nums.end());


        return ans;
    }
};

int main() {
    vector<int> nums = {1, 2, 1};

    Solution solution;

    vector<int> ans = solution.getConcatenation(nums);

    for (int x : ans) {
        cout << x << " ";
    }

    return 0;
}