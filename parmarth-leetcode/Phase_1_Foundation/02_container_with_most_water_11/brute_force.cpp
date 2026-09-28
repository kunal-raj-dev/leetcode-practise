// ============================================================
// Problem  : Container With Most Water
// Approach : Brute Force - Check Every Pair
// Time     : O(N^2) - try every pair of lines
// Space    : O(1)   - use only a few variables
// ============================================================

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

int maxArea(vector<int>& height) {
    int bestArea = 0;
    int n = static_cast<int>(height.size());

    // Every pair is a possible pair of container walls.
    for (int left = 0; left < n; left++) {
        for (int right = left + 1; right < n; right++) {
            int width = right - left;
            int waterHeight = min(height[left], height[right]);
            int area = width * waterHeight;
            bestArea = max(bestArea, area);
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

    cout << "All brute-force tests passed. Example maximum area: "
         << maxArea(example) << '\n';
    return 0;
}
