// ============================================================
// Approach 2 — Build using push_back()
// Time     : O(N)
// Space    : O(N)
// ============================================================

#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        vector<int> ans;

        for (int x : nums) {
            ans.push_back(x);
        }

        for (int x : nums) {
            ans.push_back(x);
        }

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