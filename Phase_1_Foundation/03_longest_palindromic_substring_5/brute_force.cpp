// ============================================================
// Problem  : Longest Palindromic Substring (LeetCode #5)
// Approach : Brute Force - Check All Substrings
// Time     : O(N^3) - O(N^2) substrings * O(N) palindrome check
// Space    : O(1) auxiliary space (excluding returned string)
// ============================================================

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Helper: Check if substring s[left...right] reads the same forwards and backwards.
// Time: O(L) where L = right - left + 1
bool isPalindrome(const string& s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

// Brute Force: Enumerate every possible start index i and end index j.
// For every substring s[i..j], verify if it is a palindrome.
// Keep track of the longest one discovered.
string longestPalindrome(string s) {
    int n = static_cast<int>(s.length());
    if (n <= 1) return s;

    int maxLen = 0;
    int startIdx = 0;

    // Outer loop: starting position of substring
    for (int i = 0; i < n; i++) {
        // Inner loop: ending position of substring
        for (int j = i; j < n; j++) {
            int currentLen = j - i + 1;
            // Prune: only check palindrome if this substring is longer than our best so far
            if (currentLen > maxLen && isPalindrome(s, i, j)) {
                maxLen = currentLen;
                startIdx = i;
            }
        }
    }

    return s.substr(startIdx, maxLen);
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Test Case 1: Odd-length palindrome in the middle
    string s1 = "babad";
    string res1 = longestPalindrome(s1);
    // Both "bab" and "aba" are valid answers of length 3
    assert(res1 == "bab" || res1 == "aba");

    // Test Case 2: Even-length palindrome
    string s2 = "cbbd";
    assert(longestPalindrome(s2) == "bb");

    // Test Case 3: Single character string
    string s3 = "a";
    assert(longestPalindrome(s3) == "a");

    // Test Case 4: Two different characters
    string s4 = "ac";
    string res4 = longestPalindrome(s4);
    assert(res4 == "a" || res4 == "c");

    // Test Case 5: Entire string is a palindrome
    string s5 = "racecar";
    assert(longestPalindrome(s5) == "racecar");

    // Test Case 6: All identical characters
    string s6 = "aaaa";
    assert(longestPalindrome(s6) == "aaaa");

    cout << "All brute-force tests passed successfully!\n";
    cout << "Sample result for \"babad\": " << res1 << '\n';

    return 0;
}
