// ============================================================================
// Problem  : Longest Substring Without Repeating Characters (LeetCode #3)
// URL      : https://leetcode.com/problems/longest-substring-without-repeating-characters/
// Approach : Brute Force (Generate Substrings + Duplicate Check)
// Time     : O(N^3) Pure Naive | O(N^2) Pruned Duplicate Search
// Space    : O(min(N, Sigma)) auxiliary space where Sigma is alphabet size (128 ASCII)
// ============================================================================
//
// PEDAGOGICAL PURPOSE:
// Understand the baseline human intuition before optimizing.
// By seeing exactly where redundant work occurs in the nested loops,
// we uncover the fundamental motivation for the Sliding Window pattern.
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cassert>

using namespace std;

// ============================================================================
// APPROACH 1: Pure Naive Brute Force — O(N^3) Time, O(Sigma) Space
// ============================================================================
// Logic:
// 1. Generate every possible substring bounded by index i (start) and j (end).
// 2. For each substring s[i..j], run a helper function to verify if all
//    characters are distinct.
// 3. If distinct, update maxLen = max(maxLen, j - i + 1).
// ============================================================================

// Helper function: Checks if substring s[start..end] contains all unique characters.
// Time Complexity: O(L) where L = end - start + 1.
// Space Complexity: O(Sigma) = O(128) = O(1) for the visited boolean array.
bool allUnique(const string& s, int start, int end) {
    // Array of size 128 covers standard ASCII (digits, uppercase, lowercase, symbols, space).
    // Initialized to false.
    bool visited[128] = {false};

    for (int k = start; k <= end; ++k) {
        unsigned char ch = static_cast<unsigned char>(s[k]);
        // If this character has already appeared in the current window s[start..end],
        // then the substring has a duplicate.
        if (visited[ch]) {
            return false;
        }
        visited[ch] = true;
    }
    return true;
}

int lengthOfLongestSubstring_Naive(const string& s) {
    int n = static_cast<int>(s.length());
    if (n <= 1) return n;

    int maxLen = 0;

    // Outer loop: pick start index i
    for (int i = 0; i < n; ++i) {
        // Inner loop: pick end index j
        // Optimization note: We only need to check j if the length (j - i + 1)
        // is strictly greater than our current best maxLen.
        for (int j = i + maxLen; j < n; ++j) {
            if (allUnique(s, i, j)) {
                maxLen = max(maxLen, j - i + 1);
            }
        }
    }

    return maxLen;
}

// ============================================================================
// APPROACH 2: Incremental Brute Force (Early Exit) — O(N^2) Time, O(Sigma) Space
// ============================================================================
// Bottleneck of Approach 1:
// When evaluating s[i..j], Approach 1 re-checks all characters from i to j-1,
// which we ALREADY verified were unique when we checked s[i..j-1]!
//
// Improvement:
// Fix start index i. As we advance j from i onwards, incrementally insert s[j]
// into our seen set. The moment s[j] is already in the set, NO LARGER substring
// starting at i can possibly be valid! Break out of the j loop immediately.
// ============================================================================

int lengthOfLongestSubstring_Incremental(const string& s) {
    int n = static_cast<int>(s.length());
    if (n <= 1) return n;

    int maxLen = 0;

    // For every possible starting index i
    for (int i = 0; i < n; ++i) {
        // Track characters seen so far starting from index i
        bool visited[128] = {false};

        for (int j = i; j < n; ++j) {
            unsigned char ch = static_cast<unsigned char>(s[j]);

            // If s[j] is already in our substring s[i..j-1], then s[i..j] and
            // any further substring s[i..k] (where k > j) will also contain duplicates.
            // Early break saves unnecessary computation!
            if (visited[ch]) {
                break;
            }

            visited[ch] = true;
            maxLen = max(maxLen, j - i + 1);
        }
    }

    return maxLen;
}

// ============================================================================
// Test Suite & Assertions
// ============================================================================
int main() {
    // Fast I/O
    // ios_base::sync_with_stdio(false) uncouples C++ streams from C stdio functions.
    // cin.tie(NULL) prevents automatic flushing of cout before cin reads.
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== Running Brute Force Solutions for LC #3 ===\n";

    struct TestCase {
        string input;
        int expected;
        string description;
    };

    vector<TestCase> testCases = {
        {"abcabcbb", 3, "Standard case with repeating triplets ('abc')"},
        {"bbbbb", 1, "All identical characters ('b')"},
        {"pwwkew", 3, "Answer in middle with repeat in between ('wke')"},
        {"", 0, "Empty string edge case"},
        {" ", 1, "Single space character"},
        {"au", 2, "Two distinct characters"},
        {"dvdf", 3, "Trap case: duplicate 'd' separated by 'v' ('vdf')"},
        {"tmmzuxt", 5, "Duplicate at start, longer unique later ('mzuxt')"},
        {"abcdefghijklmnopqrstuvwxyz", 26, "All unique alphabet"},
        {"abba", 2, "Palindrome with duplicate adjacent ('ab' or 'ba')"}
    };

    for (size_t idx = 0; idx < testCases.size(); ++idx) {
        const auto& tc = testCases[idx];
        int resNaive = lengthOfLongestSubstring_Naive(tc.input);
        int resIncr = lengthOfLongestSubstring_Incremental(tc.input);

        cout << "Test " << (idx + 1) << " [" << tc.description << "]:\n";
        cout << "  Input   : \"" << tc.input << "\"\n";
        cout << "  Expected: " << tc.expected << "\n";
        cout << "  Naive   : " << resNaive << "\n";
        cout << "  Incr    : " << resIncr << "\n";

        assert(resNaive == tc.expected);
        assert(resIncr == tc.expected);
        cout << "  Status  : PASSED (assert verified)\n\n";
    }

    cout << "All test cases passed successfully!\n";
    return 0;
}
