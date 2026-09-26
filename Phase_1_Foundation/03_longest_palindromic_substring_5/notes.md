# 📌 Longest Palindromic Substring — Complete Notes

**LeetCode #5:** [Longest Palindromic Substring — LeetCode](https://leetcode.com/problems/longest-palindromic-substring/)  
**Topic:** String, Two Pointers, Dynamic Programming, Manacher's Algorithm  
**Difficulty:** 🟡 Medium  
**Google-Tagged:** 🌟 Yes (All-time Google interview favorite, asked across L3/L4/Intern loops)  
**Phase:** 1 — Foundation  

---

## 🔵 LAYER 1 — What Is This Problem Asking?

### Problem Statement (Plain English)

You are given a string `s`. Your objective is to find and return the **longest contiguous slice (substring)** inside `s` that reads identically forward and backward (a **palindrome**).

If there are multiple palindromic substrings with the exact same maximum length, returning any one of them is acceptable.

### One-Line Rephrasing

> "Find the longest contiguous block of characters within string `s` that mirrors perfectly across its center."

### Visual Example

```text
Input:  s = "babad"

All palindromic substrings:
  Single chars : "b", "a", "b", "a", "d" (length 1)
  Length 3     : "bab" (indices 0..2)
  Length 3     : "aba" (indices 1..3)

Output: "bab" (or "aba", both have max length 3)
```

Another example:
```text
Input:  s = "cbbd"
Substrings: "c", "b", "b", "d", "bb"
Output: "bb" (length 2)
```

### Inputs & Outputs

```text
INPUT:
  s       → A string of English letters and/or digits (1 <= s.length <= 1000)

OUTPUT:
  string  → The longest contiguous palindromic substring
```

### Constraints & Algorithmic Implications

```text
1 <= s.length <= 1000
s consists of only digits and English letters.
```

- **$N \le 1000$**:
  - A brute-force $O(N^3)$ algorithm performs up to $(1000)^3 = 10^9$ operations $\implies$ **Time Limit Exceeded (TLE)** on standard 1-second limits ($\approx 10^8$ operations).
  - An $O(N^2)$ algorithm performs around $10^6$ operations $\implies$ **Easily passes within $< 20$ ms**.
  - An $O(N)$ algorithm (Manacher's) performs $2 \times 10^3$ operations $\implies$ **Blazing fast ($\approx 1$ ms)**.

### ⚠️ Common Beginner Traps & Misreadings

1. **Substring vs. Subsequence**:
   - A **substring** must be **contiguous** (no gaps allowed).
   - A **subsequence** can skip characters while preserving order.
   - For example, in `"pwwkew"`, `"wke"` is a substring, but `"pwke"` is only a subsequence.
   - Finding the Longest Palindromic *Subsequence* is LeetCode #516 (solved via 2D DP in $O(N^2)$ time), a completely different problem!

2. **The "Reverse and Find Longest Common Substring" Trap**:
   - Many candidates intuitively think: *"If I reverse `s` to get `s_rev`, the longest common substring between `s` and `s_rev` must be the longest palindrome!"*
   - **Counterexample**:
     ```text
     s     = "abacdfgdcaba"
     s_rev = "abacdgfdcaba"
     Longest Common Substring: "abacd" (length 5)
     Is "abacd" a palindrome? NO!
     ```
     This fails because a reversed copy of a non-palindromic substring may appear elsewhere in the original string!

3. **Odd vs. Even Palindrome Centers**:
   - An odd-length palindrome (e.g., `"racecar"`, length 7) has a single character center (`'e'`).
   - An even-length palindrome (e.g., `"noon"`, length 4) has a center situated **between** two characters (`'o'` and `'o'`).
   - Forgetting even-length centers causes solutions to fail on cases like `"cbbd"` or `"abba"`.

---

## 🟡 LAYER 2 — Concept Building ("What world does this problem live in?")

### Algorithmic Family: Palindromic Symmetry & Two Pointers

A palindrome is characterized by **bilateral symmetry**. If you place a mirror in the exact middle of a palindrome, the left half is an exact reflection of the right half:

```text
Odd Palindrome ("racecar"):
   r   a   c   [e]   c   a   r
  <-- <-- <--   ^   --> --> -->
              Center

Even Palindrome ("noon"):
   n   o  |  o   n
  <-- <-- | --> -->
       Center
```

### Visual Mental Model: The $2N - 1$ Centers

For a string of length $N$:
- There are $N$ individual character centers (for odd-length palindromes).
- There are $N - 1$ spaces between adjacent characters (for even-length palindromes).
- Total possible centers = $N + (N - 1) = \mathbf{2N - 1}$.

```text
String:   b     a     b     a     d
Indices:  0     1     2     3     4

Odd:     [0]   [1]   [2]   [3]   [4]        --> 5 centers
Even:       (0,1) (1,2) (2,3) (3,4)        --> 4 centers
Total Centers = 5 + 4 = 9 centers (= 2 * 5 - 1)
```

### Real-World Analogy: Dropping a Pebble in Still Water

Think of each center as a point where a pebble hits a pond. The ripple expands outward symmetrically one ring at a time:
- Step 0: Pebble hits center.
- Step 1: Does the 1st ring match on both sides? Yes $\implies$ expand.
- Step 2: Does the 2nd ring match on both sides? Yes $\implies$ expand.
- Step 3: Mismatch detected or hit the shore (string boundary) $\implies$ stop!

### Prerequisite C++ Tools

1. **`std::string::substr(size_type pos = 0, size_type count = npos)`**:
   - Extracts a substring starting at index `pos` with length `count`.
   - **Cost:** $O(\text{count})$ time due to copying characters.
   - *Pro-Tip:* Do not call `.substr()` inside your loops! Only record `start` and `maxLen`, then call `.substr()` once at the very end.

2. **Integer Division Truncation**:
   - In C++, integer division truncates toward zero: `(3 - 1) / 2 = 1`, and `(4 - 1) / 2 = 1`.
   - We will exploit this exact mathematical property to unify start-index calculations for both odd and even lengths.

---

## 🟠 LAYER 3 — Problem-Solving Mindset ("How do I think my way to a solution?")

### Approach 1: Brute Force ($O(N^3)$ Time, $O(1)$ Space)

**Intuition:**
1. Generate every possible substring $s[i \dots j]$ where $0 \le i \le j < N$.
2. Check if $s[i \dots j]$ is a palindrome by comparing from outer edges inward.
3. Track the longest valid palindrome.

```text
Number of substrings = N * (N + 1) / 2  -->  O(N^2)
Checking each substring of length L    -->  O(L) = O(N)
Total Time Complexity                 -->  O(N^3)
```

**Why is Brute Force Wasteful? (The Inner Monologue)**:
> *"When I check whether `s[1..5]` ('xabax') is a palindrome, I start comparing from scratch. But earlier, I already checked `s[2..4]` ('aba')! Why am I throwing away previously discovered symmetry? If `s[2..4]` is already a palindrome, then `s[1..5]` is a palindrome if and only if `s[1] == s[5]`!"*

This realization branches into two superior techniques:
1. **Dynamic Programming** (Top-down or Bottom-up): Store smaller palindrome subproblems in a 2D table.
2. **Expand Around Center**: Grow outward from the center instead of checking inward from the edges.

---

### Approach 2: 2D Dynamic Programming ($O(N^2)$ Time, $O(N^2)$ Space)

Let `dp[i][j]` be a boolean: is substring $s[i \dots j]$ a palindrome?

**Recurrence Relation:**
```text
dp[i][j] = true  IF  (s[i] == s[j]) AND ( (j - i < 3) OR dp[i + 1][j - 1] )
```

- Base Case 1: Substrings of length 1 ($i = j$) are always palindromes (`dp[i][i] = true`).
- Base Case 2: Substrings of length 2 ($j = i + 1$) are palindromes if $s[i] == s[i + 1]$ (`j - i < 3` covers this: $j - i = 1 < 3$).
- Substrings of length $\ge 3$: $s[i \dots j]$ is a palindrome if end characters match and the inner substring `dp[i + 1][j - 1]` is also a palindrome.

**Why Expand Around Center Beats DP for Interviews:**
- DP requires an $N \times N$ matrix $\implies O(N^2)$ extra memory ($1000 \times 1000 = 1\text{ MB}$, but still $O(N^2)$ space).
- Expand Around Center uses **$O(1)$ auxiliary space**!

---

### Approach 3: Expand Around Center (The Interview Gold Standard)

Instead of searching from endpoints inward, we pick a center and expand outward:

```text
Odd Center at index i:
  left = i, right = i
  Expand while left >= 0 and right < n and s[left] == s[right]:
    left--, right++

Even Center between i and i + 1:
  left = i, right = i + 1
  Expand while left >= 0 and right < n and s[left] == s[right]:
    left--, right++
```

#### Step-by-Step Mathematical Derivation of Start Index

When expansion halts, `s[left] != s[right]` (or bounds were exceeded):
- The last valid matching left index was: `left + 1`
- The last valid matching right index was: `right - 1`
- Length of valid palindrome:
  $$\text{Length} = (\text{right} - 1) - (\text{left} + 1) + 1 = \mathbf{right - left - 1}$$

Now, given `maxLen` and the loop index `i`:
- **For odd length $L$ centered at $i$**:
  The palindrome extends $(L - 1) / 2$ characters to the left.
  $$\text{start} = i - \frac{L - 1}{2}$$
  *Example:* `s = "babad"`, center $i = 1$ ('a'), $L = 3 \implies \text{start} = 1 - (3 - 1) / 2 = 0 \implies s[0..2] = \text{"bab"}$.

- **For even length $L$ centered between $i$ and $i + 1$**:
  The left center is at $i$. The palindrome extends $(L / 2) - 1$ characters to the left of $i$.
  Since $L$ is even, $(L - 1) / 2$ evaluates to $(L / 2) - 1$ under integer division!
  $$\text{start} = i - \frac{L - 1}{2}$$
  *Example:* `s = "cbbd"`, center $i = 1$ ('b'), $L = 2 \implies \text{start} = 1 - (2 - 1) / 2 = 1 - 0 = 1 \implies s[1..2] = \text{"bb"}$.

**Conclusion:** The exact same formula `start = i - (maxLen - 1) / 2` works unconditionally for both odd and even palindromes!

---

### Approach 4: Manacher's Algorithm ($O(N)$ Time, $O(N)$ Space) — Google L4+ Follow-up

Can we do better than $O(N^2)$? **Yes, in linear $O(N)$ time!**

Glenn Manacher discovered this in 1975:
1. **Preprocess string with delimiters** (e.g. `"aba"` $\implies$ `"^#a#b#a#$"`).
   - This ingeniously converts all palindromes (even and odd) into odd-length palindromes!
   - Sentinels `'^'` and `'$'` prevent array boundary checking.
2. Maintain:
   - `center`: The center of the palindrome that reaches the furthest to the right.
   - `right`: The rightmost boundary ($center + radius$) reached so far.
3. For each index $i$:
   - Find its reflection across `center`: $i_{mirror} = 2 \times center - i$.
   - By palindrome symmetry, if $i < right$, the palindrome radius at $i$ is at least $\min(right - i, p[i_{mirror}])$!
   - Only expand character-by-character when extending past `right`.
   - Every character is matched successfully at most once when extending `right`, leading to a strict $O(N)$ runtime.

---

## 🔴 LAYER 4 — Implementation ("Let's write the C++ code")

### 1. Brute Force Implementation (`brute_force.cpp`)

```cpp
#include <bits/stdc++.h>
using namespace std;

bool isPalindrome(const string& s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) return false;
        left++;
        right--;
    }
    return true;
}

string longestPalindromeBruteForce(string s) {
    int n = static_cast<int>(s.length());
    if (n <= 1) return s;

    int maxLen = 0;
    int startIdx = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int currentLen = j - i + 1;
            if (currentLen > maxLen && isPalindrome(s, i, j)) {
                maxLen = currentLen;
                startIdx = i;
            }
        }
    }
    return s.substr(startIdx, maxLen);
}
```

- **Time Complexity:** $O(N^3)$ — $\frac{N(N+1)}{2}$ substrings, each verified in $O(N)$ time.
- **Space Complexity:** $O(1)$ auxiliary space.

---

### 2. Primary Optimal Solution: Expand Around Center (`optimized.cpp`)

```cpp
#include <bits/stdc++.h>
using namespace std;

// Expands outward from [left, right] while characters match
// Returns the length of the palindrome
int expandAroundCenter(const string& s, int left, int right) {
    int n = static_cast<int>(s.length());
    while (left >= 0 && right < n && s[left] == s[right]) {
        left--;
        right++;
    }
    // Length is: (right - 1) - (left + 1) + 1 = right - left - 1
    return right - left - 1;
}

string longestPalindrome(string s) {
    int n = static_cast<int>(s.length());
    if (n <= 1) return s;

    int start = 0;
    int maxLen = 1;

    for (int i = 0; i < n; i++) {
        // Odd length center: s[i]
        int len1 = expandAroundCenter(s, i, i);

        // Even length center: between s[i] and s[i + 1]
        int len2 = expandAroundCenter(s, i, i + 1);

        int bestCurrent = max(len1, len2);

        if (bestCurrent > maxLen) {
            maxLen = bestCurrent;
            start = i - (bestCurrent - 1) / 2;
        }
    }

    return s.substr(start, maxLen);
}
```

- **Time Complexity:** $O(N^2)$ — $2N - 1$ centers, each can expand up to $O(N)$ characters.
- **Space Complexity:** $O(1)$ auxiliary space — only integer indices and lengths are maintained.

---

### 3. Comparison of All 4 Approaches

| Approach | Time Complexity | Auxiliary Space | Pros | Cons | Interview Recommendation |
|---|:---:|:---:|---|---|---|
| **Brute Force** | $O(N^3)$ | $O(1)$ | Simple to conceptualize | TLE on $N = 1000$ | State first as baseline |
| **2D Dynamic Programming** | $O(N^2)$ | $O(N^2)$ | Clean subproblem formulation | High memory footprint ($10^6$ booleans) | Good to mention, don't stop here |
| **Expand Around Center** | $O(N^2)$ | $O(1)$ | Optimal space, extremely fast in practice | Quadratic worst case on "aaaaa" | ⭐ **Must-code interview standard** |
| **Manacher's Algorithm** | $O(N)$ | $O(N)$ | Mathematically optimal $O(N)$ | Complex implementation with sentinels | 🌟 Discuss as Google follow-up |

---

### Edge Cases Checked

1. **Single Character** (`s = "a"`): Loops run once, returns `"a"`.
2. **Two Distinct Characters** (`s = "ac"`): Odd checks find length 1, returns `"a"`.
3. **Two Identical Characters** (`s = "aa"`): Even center at `(0, 1)` yields length 2, returns `"aa"`.
4. **Entire String is a Palindrome** (`s = "racecar"`): Expands to full string, returns `"racecar"`.
5. **All Characters Identical** (`s = "aaaa"`): Handles all overlapping odd and even centers correctly, returns `"aaaa"`.
6. **No Palindromes Longer Than 1** (`s = "abcdef"`): Correctly returns the first character `"a"`.

---

## 🟣 LAYER 5 — Pattern Extraction ("What did we actually learn?")

### Reusable Pattern: Expand Around Center

```text
Whenever a problem asks for:
  - Longest Palindromic Substring
  - Count of all Palindromic Substrings
  - Symmetry verification around local anchor points

Think:
  1. Palindromes are defined by their centers.
  2. There are 2N - 1 centers (N odd, N - 1 even).
  3. Expand outward with two pointers (L, R) while s[L] == s[R].
  4. Yields O(N^2) time with O(1) space, avoiding expensive DP state tables.
```

### Reusable Code Template

```cpp
int expand(const string& s, int L, int R) {
    while (L >= 0 && R < (int)s.size() && s[L] == s[R]) {
        L--;
        R++;
    }
    return R - L - 1; // Valid palindrome length
}

// Inside driver:
for (int i = 0; i < n; i++) {
    int oddLen  = expand(s, i, i);
    int evenLen = expand(s, i, i + 1);
    // update global best
}
```

### 🧠 Must Memorize vs. Re-derive

- **Must Memorize:**
  - Number of centers = $2N - 1$.
  - The expansion termination condition and length formula: `length = right - left - 1`.
  - The start index formula: `start = center - (length - 1) / 2`.
- **Re-derive Each Time:**
  - The sentinel character insertion if implementing Manacher's algorithm.

---

### Similar Problems to Master Next

| # | Problem | Core Technique | Link |
|---|---|---|---|
| 647 | **Palindromic Substrings** | Expand Around Center (count each valid expansion) | [LeetCode 647](https://leetcode.com/problems/palindromic-substrings/) |
| 516 | **Longest Palindromic Subsequence** | 2D Dynamic Programming (interval DP) | [LeetCode 516](https://leetcode.com/problems/longest-palindromic-subsequence/) |
| 125 | **Valid Palindrome** | Inward Two Pointers | [LeetCode 125](https://leetcode.com/problems/valid-palindrome/) |
| 214 | **Shortest Palindrome** | KMP Prefix Table / Rolling Hash | [LeetCode 214](https://leetcode.com/problems/shortest-palindrome/) |
| 131 | **Palindrome Partitioning** | Backtracking + Palindrome DP table | [LeetCode 131](https://leetcode.com/problems/palindrome-partitioning/) |
