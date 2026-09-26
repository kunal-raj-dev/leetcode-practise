// ============================================================================
// Problem  : Longest Substring Without Repeating Characters (LeetCode #3)
// URL      : https://leetcode.com/problems/longest-substring-without-repeating-characters/
// Approach : Sliding Window (Two Pointers) + Direct Access Table / Hash Map
// Time     : O(N) Single Pass (at most N iterations)
// Space    : O(Sigma) auxiliary space where Sigma = 128 (ASCII alphabet size) -> O(1)
// ============================================================================
//
// CORE INSIGHTS:
// 1. A valid window is defined as s[left...right] where all characters are distinct.
// 2. Instead of restarting from scratch when a duplicate is found (which causes
//    redundant O(N^2) / O(N^3) work), we slide the window boundaries monotonically:
//    - Expand the right boundary to absorb new characters.
//    - When s[right] duplicates an existing character inside the active window,
//      we slide/jump the left boundary past the previous occurrence.
// 3. THE "JUMP" OPTIMIZATION (O(N) single pass):
//    By maintaining an array/hash map of the LAST SEEN index of every character,
//    we can update left in O(1) using:
//        left = max(left, lastSeen[s[right]] + 1);
//    CRITICAL: The max() is non-negotiable! If s[right] was seen at an index
//    strictly before our current 'left' boundary, jumping backwards would
//    destroy the validity of the current window (e.g. in "abba").
// 4. DIRECT ACCESS TABLE vs HASH MAP:
//    Since input characters are standard ASCII (English letters, digits, symbols,
//    space), an array of size 128 provides direct indexing:
//    - Zero hash collisions
//    - Zero dynamic heap allocations
//    - L1 cache-resident (128 ints = 512 bytes)
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cassert>

using namespace std;

// ============================================================================
// APPROACH 1: Sliding Window with Dynamic Shrinking (Two Pointers + Visited Set)
// Time Complexity : O(2N) = O(N) — each char added by 'right' once, removed by 'left' at most once
// Space Complexity: O(Sigma) = O(128) = O(1) space
// ============================================================================
int lengthOfLongestSubstring_ShrinkingWindow(const string& s) {
    int n = static_cast<int>(s.length());
    if (n <= 1) return n;

    // Direct access boolean array acting as an O(1) hash set for ASCII [0..127]
    bool inWindow[128] = {false};

    int left = 0;
    int maxLen = 0;

    for (int right = 0; right < n; ++right) {
        unsigned char ch = static_cast<unsigned char>(s[right]);

        // If ch is already in our window, shrink from the left until ch is removed
        while (inWindow[ch]) {
            unsigned char leftChar = static_cast<unsigned char>(s[left]);
            inWindow[leftChar] = false;
            left++;
        }

        // Add s[right] to our window
        inWindow[ch] = true;

        // Window s[left..right] is guaranteed to have all unique characters
        maxLen = max(maxLen, right - left + 1);
    }

    return maxLen;
}

// ============================================================================
// APPROACH 2: Optimal Single-Pass Sliding Window with Direct Access Table
// Time Complexity : O(N) — 'right' advances exactly N times; 'left' only jumps forward
// Space Complexity: O(Sigma) = O(128) = O(1) fixed space
// ============================================================================
int lengthOfLongestSubstring_Optimal(const string& s) {
    int n = static_cast<int>(s.length());
    if (n <= 1) return n;

    // lastSeen[c] stores the 0-based index where character 'c' was last encountered.
    // Initialized to -1 to denote "not yet seen".
    vector<int> lastSeen(128, -1);

    int left = 0;
    int maxLen = 0;

    for (int right = 0; right < n; ++right) {
        unsigned char ch = static_cast<unsigned char>(s[right]);

        // If character 'ch' has been seen before:
        // We want to jump 'left' to the position immediately following its last occurrence: lastSeen[ch] + 1.
        // HOWEVER, if lastSeen[ch] < left, that previous occurrence is OUTSIDE our active window!
        // Taking max(left, lastSeen[ch] + 1) prevents 'left' from regressing backward.
        if (lastSeen[ch] != -1) {
            left = max(left, lastSeen[ch] + 1);
        }

        // Record the current position of character 'ch'
        lastSeen[ch] = right;

        // Calculate current window length and update global maximum
        maxLen = max(maxLen, right - left + 1);
    }

    return maxLen;
}

// ============================================================================
// APPROACH 3: Google Follow-Up — Reconstructing the Actual Longest Substring
// Returns: The actual substring itself, rather than just its length.
// Time Complexity : O(N) scan + O(K) substring copy where K = length of result
// Space Complexity: O(1) auxiliary space (excluding returned string)
// ============================================================================
string longestSubstringWithoutRepeatingString(const string& s) {
    int n = static_cast<int>(s.length());
    if (n <= 1) return s;

    vector<int> lastSeen(128, -1);

    int left = 0;
    int maxLen = 0;
    int bestStart = 0;

    for (int right = 0; right < n; ++right) {
        unsigned char ch = static_cast<unsigned char>(s[right]);

        if (lastSeen[ch] != -1) {
            left = max(left, lastSeen[ch] + 1);
        }

        lastSeen[ch] = right;

        int currentLen = right - left + 1;
        if (currentLen > maxLen) {
            maxLen = currentLen;
            bestStart = left;
        }
    }

    return s.substr(bestStart, maxLen);
}

// ============================================================================
// Comprehensive Test Suite & Assertions
// ============================================================================
int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== Running Optimal Sliding Window Solutions for LC #3 ===\n\n";

    struct TestCase {
        string input;
        int expectedLen;
        string description;
    };

    vector<TestCase> testCases = {
        {"abcabcbb", 3, "Standard repeating triplets ('abc')"},
        {"bbbbb", 1, "All identical characters ('b')"},
        {"pwwkew", 3, "Middle answer with repeated chars around ('wke')"},
        {"", 0, "Empty string"},
        {" ", 1, "Single whitespace"},
        {"au", 2, "Two distinct characters"},
        {"dvdf", 3, "Critical non-adjacent duplicate ('vdf')"},
        {"abba", 2, "Critical pointer regression test ('ab' or 'ba')"},
        {"tmmzuxt", 5, "Initial duplicate followed by longer unique ('mzuxt')"},
        {"bbtablud", 6, "Mixed duplicates with space for expansion ('tablud')"},
        {"!@#$%^&*()", 10, "Special symbols and punctuation"},
        {"a b c a b c", 3, "Spaces alternating with characters ('a b' or ' b c')"},
        {"abcdefghijklmnopqrstuvwxyz", 26, "Full lowercase alphabet"}
    };

    for (size_t i = 0; i < testCases.size(); ++i) {
        const auto& tc = testCases[i];
        int resShrink = lengthOfLongestSubstring_ShrinkingWindow(tc.input);
        int resOptimal = lengthOfLongestSubstring_Optimal(tc.input);
        string subStr = longestSubstringWithoutRepeatingString(tc.input);

        cout << "Test Case " << (i + 1) << " [" << tc.description << "]:\n";
        cout << "  Input           : \"" << tc.input << "\"\n";
        cout << "  Expected Length : " << tc.expectedLen << "\n";
        cout << "  Shrink Window   : " << resShrink << "\n";
        cout << "  Optimal Jump    : " << resOptimal << "\n";
        cout << "  Actual Substring: \"" << subStr << "\" (Length: " << subStr.length() << ")\n";

        assert(resShrink == tc.expectedLen);
        assert(resOptimal == tc.expectedLen);
        assert(static_cast<int>(subStr.length()) == tc.expectedLen);

        cout << "  Verification    : PASSED (all assertions validated)\n\n";
    }

    cout << "All 13 test cases passed with zero errors!\n";
    return 0;
}
