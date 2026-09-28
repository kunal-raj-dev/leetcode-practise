# Approach 2 — Binary Search Partition (Optimized)

## 1. Core Idea

Instead of merging the two arrays, binary search for the correct partition point on the smaller array. The partition splits both arrays such that the combined left halves contain exactly `⌈(m+n)/2⌉` elements, and every left element ≤ every right element. The median is computed directly from the four boundary values.

## 2. Why It Works

The median divides the merged sorted array into two equal halves. If we take `i` elements from `nums1` and `j = half - i` elements from `nums2` for the left half, we need:

- `maxLeft1 ≤ minRight2` (everything from nums1's left ≤ everything from nums2's right)
- `maxLeft2 ≤ minRight1` (everything from nums2's left ≤ everything from nums1's right)

Within-array ordering is guaranteed by the sorted property. So we only check the cross-boundaries.

Because the condition is monotonic (increasing `i` makes `maxLeft1` grow and `minRight2` shrink), binary search works.

## 3. How To Think About It

1. "The median splits the data into two halves. I need the boundary values."
2. "If I fix how many elements come from nums1's left side (`i`), the number from nums2's left side is determined (`j = half - i`)."
3. "So I have one degree of freedom: `i`. I can binary search over it."
4. "The valid partition condition is monotonic — if `maxLeft1` is too big, I need fewer from nums1."
5. "Search the smaller array so the search space is minimized."

## 4. Visual Trace

```
nums1 = [1, 5, 9]     m = 3
nums2 = [2, 6, 10]    n = 3
half = (3 + 3 + 1) / 2 = 3

Binary search on nums1 (m=3, equal size, either works):
low=0, high=3

--- Iteration 1 ---
i = (0+3)/2 = 1
j = 3 - 1 = 2

  nums1: [1 | 5, 9]     maxLeft1 = 1,   minRight1 = 5
  nums2: [2, 6 | 10]    maxLeft2 = 6,   minRight2 = 10

  Check: maxLeft1(1) ≤ minRight2(10) ✓
         maxLeft2(6) ≤ minRight1(5)  ✗ → 6 > 5, need more from nums1
  Move: low = 1 + 1 = 2

--- Iteration 2 ---
i = (2+3)/2 = 2
j = 3 - 2 = 1

  nums1: [1, 5 | 9]     maxLeft1 = 5,   minRight1 = 9
  nums2: [2 | 6, 10]    maxLeft2 = 2,   minRight2 = 6

  Check: maxLeft1(5) ≤ minRight2(6) ✓
         maxLeft2(2) ≤ minRight1(9) ✓ → FOUND!

  Total = 6 (even)
  median = (max(5, 2) + min(9, 6)) / 2.0 = (5 + 6) / 2.0 = 5.5 ✓
```

## 5. Algorithm / Pseudocode

```
function findMedianSortedArrays(nums1, nums2):
    // Ensure nums1 is the smaller array
    if len(nums1) > len(nums2):
        swap(nums1, nums2)

    m = len(nums1), n = len(nums2)
    low = 0, high = m
    half = (m + n + 1) / 2

    while low <= high:
        i = (low + high) / 2       // partition index in nums1
        j = half - i                // partition index in nums2

        maxLeft1  = (i == 0) ? -∞ : nums1[i-1]
        minRight1 = (i == m) ? +∞ : nums1[i]
        maxLeft2  = (j == 0) ? -∞ : nums2[j-1]
        minRight2 = (j == n) ? +∞ : nums2[j]

        if maxLeft1 <= minRight2 AND maxLeft2 <= minRight1:
            if (m + n) is odd:
                return max(maxLeft1, maxLeft2)
            else:
                return (max(maxLeft1, maxLeft2) + min(minRight1, minRight2)) / 2.0
        else if maxLeft1 > minRight2:
            high = i - 1    // took too many from nums1
        else:
            low = i + 1     // took too few from nums1
```

## 6. Complexity

### Time

- **Claim**: O(log(min(m, n)))
- **Proof**: We binary search over the partition index `i ∈ [0, m]` where `m = min(m, n)`. Each iteration does O(1) work (comparisons, index arithmetic). The search space halves each iteration. Total iterations: `⌈log₂(m + 1)⌉`. Therefore: O(log(min(m, n))).
- **Lower bound**: Any comparison-based algorithm for this problem requires Ω(log(min(m, n))) comparisons (information-theoretic argument: the answer depends on `m + 1` possible partition positions).

### Space

- **Output space**: O(1) — returns a single `double`.
- **Auxiliary space**: O(1) — only uses a constant number of integer variables (`low`, `high`, `i`, `j`, `maxLeft1`, etc.).
- **Interviewer note**: "This is optimal in both time and space. No further optimization is possible."

## 7. C++ Mechanics

- `INT_MIN` / `INT_MAX` (`<climits>`): Used as sentinels. Safe because the constraint says values are in `[-10^6, 10^6]`, well within `int` range.
- **Recursive swap trick**: `if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1);` — elegantly ensures we always search on the smaller array. This adds one extra function call, not a loop.
- `(m + n + 1) / 2`: The `+1` biases the left half to have one extra element when `m+n` is odd. This simplifies the odd-case median to just `max(maxLeft1, maxLeft2)`.
- `/ 2.0` for the even case: Forces `double` division.

## 8. Edge Cases

| Case | `i` and `j` values | Sentinel behavior |
|:-----|:--------------------|:-------------------|
| `nums1` empty (`m=0`) | `i=0`, `j=half` | `maxLeft1 = INT_MIN`, `minRight1 = INT_MAX`; entire median from `nums2` |
| `nums2` empty (`n=0`) | After swap, same as above | Same sentinel handling |
| All elements of `nums1` < all of `nums2` | `i=m`, `j=half-m` | `minRight1 = INT_MAX`; partition at the boundary |
| All elements of `nums1` > all of `nums2` | `i=0`, `j=half` | `maxLeft1 = INT_MIN`; partition at the boundary |
| Single element each | `m=1, n=1, half=1` | One iteration: `i=0` or `i=1` |

## 9. Common Mistakes

1. **Forgetting to search on the smaller array**: If you search on the larger array, `j = half - i` can become negative, causing out-of-bounds access.
2. **Wrong half formula**: Using `(m + n) / 2` instead of `(m + n + 1) / 2` breaks the odd-case median extraction.
3. **Sentinel confusion**: Using `0` or `-1` instead of `INT_MIN`/`INT_MAX` fails when actual values are 0 or negative.
4. **Off-by-one in binary search bounds**: `high` should be `m` (not `m-1`) because taking all `m` elements from `nums1` is valid.
5. **Not handling the even/odd median formula correctly**: Odd → `max(maxLeft1, maxLeft2)`. Even → average with `min(minRight1, minRight2)`.

## 10. When To Prefer This Approach

- **Always** when the problem or interviewer requires O(log) time.
- When arrays are very large and O(m+n) merge is too slow.
- This is the **expected interview answer** for this problem at Google and similar companies.
- The pattern generalizes to "find the k-th element in two sorted arrays" by setting `half = k`.
