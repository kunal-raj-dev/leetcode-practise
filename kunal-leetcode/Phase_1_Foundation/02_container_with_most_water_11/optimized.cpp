// ============================================================
// Problem  : Container With Most Water
// Approach : Greedy Two Pointers
// Time     : O(N) - each pointer moves inward at most N times
// Space    : O(1) - use only a few variables
// ============================================================
// CORE INSIGHT:
// The shorter wall limits the water. With a narrower width,
// keeping that wall cannot improve the area, so move it inward.
// ============================================================

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

int maxArea(vector<int>& height) {
    int left = 0;
    int right = static_cast<int>(height.size()) - 1;
    int bestArea = 0;

    while (left < right) {
        int width = right - left;
        int waterHeight = min(height[left], height[right]);
        int area = width * waterHeight;
        bestArea = max(bestArea, area);

        // Only replacing the shorter wall can possibly raise the limit.
        if (height[left] <= height[right]) {
            left++;
        } else {
            right--;
        }
    }

    return bestArea;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> example = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    vector<int> twoLines = {1, 1};
    vector<int> zeroHeights = {0, 0, 0};
    vector<int> shortWideContainer = {1, 2, 4, 3};

    assert(maxArea(example) == 49);
    assert(maxArea(twoLines) == 1);
    assert(maxArea(zeroHeights) == 0);
    assert(maxArea(shortWideContainer) == 4);

    cout << "All two-pointer tests passed. Example maximum area: "
         << maxArea(example) << '\n';
    return 0;
}
