// ============================================================
// Problem  : Longest Palindromic Substring (LeetCode #5)
// Approach : Expand Around Center (Two Pointers) + Manacher's (O(N))
// Time     : O(N^2) for Expand Around Center | O(N) for Manacher
// Space    : O(1) auxiliary for Expand Around Center | O(N) for Manacher
// ============================================================
// CORE INSIGHT:
// A palindrome mirrors around its center. There are 2N - 1 possible
// centers (N single-character centers for odd palindromes, and N - 1
// between-character centers for even palindromes).
// Expanding outward from each center takes O(1) extra space!
// ============================================================

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
using namespace std;

// Helper: Expands outward from [left, right] as long as characters match.
// Returns the {start, end} index pair of the valid palindromic substring found.
// Returning a pair avoids the subtle length-to-start-index arithmetic in the caller,
// making the logic self-documenting and less error-prone.
pair<int, int> expandAroundCenter(const string& s, int left, int right) {
    int n = static_cast<int>(s.length());
    while (left >= 0 && right < n && s[left] == s[right]) {
        left--;
        right++;
    }
    // Loop breaks when s[left] != s[right] or indices went out of bounds.
    // The valid palindrome was bounded by (left + 1) and (right - 1).
    return {left + 1, right - 1};
}

// -------------------------------------------------------------------------
// PRIMARY INTERVIEW SOLUTION: Expand Around Center (O(N^2) Time, O(1) Space)
// -------------------------------------------------------------------------
string longestPalindrome(const string& s) {
    int n = static_cast<int>(s.length());
    if (n == 0) return "";
    if (n == 1) return s;

    int start = 0;
    int maxLen = 1;

    for (int i = 0; i < n; i++) {
        // Case 1: Odd-length palindrome centered at s[i] (e.g. "aba", center 'b')
        auto [l1, r1] = expandAroundCenter(s, i, i);

        // Case 2: Even-length palindrome centered between s[i] and s[i+1] (e.g. "abba")
        auto [l2, r2] = expandAroundCenter(s, i, i + 1);

        // Pick whichever expansion gave us the longer palindrome
        if (r1 - l1 + 1 > maxLen) {
            maxLen = r1 - l1 + 1;
            start = l1;
        }
        if (r2 - l2 + 1 > maxLen) {
            maxLen = r2 - l2 + 1;
            start = l2;
        }
    }

    return s.substr(start, maxLen);
}

// -------------------------------------------------------------------------
// ADVANCED BONUS: Manacher's Algorithm (O(N) Time, O(N) Space)
// -------------------------------------------------------------------------
// Used when N <= 10^5 or in competitive programming / Google L4+ follow-ups.
string longestPalindromeManacher(const string& s) {
    if (s.empty()) return "";

    // 1. Transform s into t with separators to unify odd and even palindromes.
    // E.g., "aba" -> "^#a#b#a#$"
    // '^' and '$' serve as sentinel bounds to avoid boundary checking.
    string t = "^";
    for (char c : s) {
        t += '#';
        t += c;
    }
    t += "#$";

    int m = static_cast<int>(t.length());
    vector<int> p(m, 0); // p[i] stores the radius of the palindrome centered at i
    int center = 0;       // Center of the palindrome extending furthest right
    int right = 0;        // Rightmost boundary of this palindrome

    int maxLen = 0;
    int centerIndex = 0;

    for (int i = 1; i < m - 1; i++) {
        int iMirror = 2 * center - i; // Reflection of i with respect to center

        // If within current right boundary, copy reflected radius up to boundary
        if (right > i) {
            p[i] = min(right - i, p[iMirror]);
        } else {
            p[i] = 0;
        }

        // Attempt to expand beyond the currently known radius
        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) {
            p[i]++;
        }

        // If expanded past the right boundary, update center and right
        if (i + p[i] > right) {
            center = i;
            right = i + p[i];
        }

        // Track maximum palindrome radius found
        if (p[i] > maxLen) {
            maxLen = p[i];
            centerIndex = i;
        }
    }

    // Map back to original string:
    // Original start index = (centerIndex - maxLen) / 2
    int start = (centerIndex - maxLen) / 2;
    return s.substr(start, maxLen);
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Test cases: {input, expected_length}
    // Both algorithms must agree on palindrome length (answer may differ for ties)
    vector<pair<string, int>> testCases = {
        {"babad",           3},
        {"cbbd",            2},
        {"a",               1},
        {"ac",              1},
        {"racecar",         7},
        {"aaaa",            4},
        {"forgeeksskeegfor",10},  // "geeksskeeg" is the longest palindrome
        {"abacdfgdcaba",    3},
        {"",                0},  // Empty string edge case
        {"abcde",           1},  // No palindrome longer than 1
        {"aaaaaa",          6},  // All identical
    };

    for (const auto& [s, expectedLen] : testCases) {
        string resOptimal  = longestPalindrome(s);
        string resManacher = longestPalindromeManacher(s);

        // Both must return a palindrome of the expected maximum length
        assert((int)resOptimal.length()  == expectedLen);
        assert((int)resManacher.length() == expectedLen);

        // Both must agree on palindrome length (may differ in which one for ties)
        assert(resOptimal.length() == resManacher.length());

        cout << "Input: \"" << s << "\" -> Output: \"" << resOptimal
             << "\" (Length: " << resOptimal.length() << ")\n";
    }

    cout << "\nAll test cases passed for both Expand Around Center and Manacher's Algorithm!\n";
    return 0;
}
