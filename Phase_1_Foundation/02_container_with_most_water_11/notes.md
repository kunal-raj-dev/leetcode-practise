# Container With Most Water — Complete Notes

**LeetCode #11:** [Container With Most Water](https://leetcode.com/problems/container-with-most-water/)
**Topic:** Array, Two Pointers, Greedy
**Difficulty:** Medium
**Google-Tagged:** No (not indicated in the provided problem entry)
**Phase:** 1 — Foundation

---

## Layer 1 — What Is This Problem Asking?

### Plain-English Statement

You are given vertical lines at positions along the x-axis. The array `height` tells you how tall each line is. Choose two different lines to form the sides of a container; the x-axis is its bottom. Return the greatest amount of water that any pair can hold.

The water level is limited by the shorter line, and the width is the distance between the lines:

```text
area = width * min(leftHeight, rightHeight)
```

### Inputs and Output

```text
Input:  height[i] = height of the line at position i
Output: the maximum area of water formed by any two lines
```

The array indices are the horizontal positions. We return the area, not the indices or the selected heights.

### Constraints and Implications

The standard LeetCode constraints are `2 <= height.length <= 10^5` and `0 <= height[i] <= 10^4`. Checking every pair takes quadratic time and can require billions of checks at the largest input. We need a linear-time approach. The maximum possible area is at most about `10^9`, which fits in a signed 32-bit `int` under these constraints.

### Dry Run by Hand

```text
height = [1,8,6,2,5,4,8,3,7]

Start with indices 0 and 8:
  width = 8, shorter height = 1, area = 8 * 1 = 8

Move the shorter side inward. Indices 1 and 8:
  width = 7, shorter height = 7, area = 7 * 7 = 49

Continue checking pairs selected by the two-pointer rule. No later pair is larger.

Maximum area = 49
```

### Common Traps

1. Using the taller line as the water height. Water spills over the shorter wall, so use the minimum height.
2. Using the number of lines between the walls as width. Width is `rightIndex - leftIndex`.
3. Moving the taller pointer in the optimized solution. That does not improve the limiting height and always makes the width smaller.
4. Confusing this with trapping rain water. Here we choose exactly two boundaries; we do not add water across every position.

**One-sentence rephrase:** Choose two array positions to maximize their horizontal distance multiplied by the shorter of their heights.

---

## Layer 2 — What Concept Does This Use?

### Data Structure and Algorithm Family

This is an **array + two pointers + greedy** problem. The pointers begin at opposite ends of the array and move toward one another. Greedy means we make a locally justified choice at each step: discard the shorter boundary because it cannot lead to a better pair while the other boundary stays fixed.

### Why the Area Formula Works

A container's water cannot rise above its shorter wall. If the walls have heights 8 and 3 and are 6 units apart, the area is `6 * 3 = 18`, not `6 * 8`.

```text
left wall                         right wall
   |                                  |
   |                                  |  height 7
   |~~~~~~~~ water level ~~~~~~~~~~~~|
   |  height 4                       |
   +----------------------------------+
              width = R - L
```

### Real-World Analogy

Imagine carrying water between two posts with a flat floor. The posts are the walls; water leaks over whichever post is shorter. Moving a post inward narrows the container. To have any chance of compensating for that lost width, we must try to find a taller replacement for the shorter wall.

### C++ Prerequisites

- `vector<int>` stores the line heights in order.
- `min(a, b)` returns the smaller of the two wall heights.
- `left` and `right` are integer indices. The loop continues while `left < right`, so the same line is never used twice.
- `max(bestArea, area)` keeps the largest area seen so far.

---

## Layer 3 — How Do We Think Our Way to a Solution?

### Begin With Brute Force

Try every possible pair of lines. For each pair `(left, right)`, calculate `(right - left) * min(height[left], height[right])`, then keep the maximum. This directly follows the definition and gives us a correctness baseline.

### Socratic Checkpoints

Before reading the answers, pause and ask:

- What is repeated in the brute-force method? We recompute areas for many pairs.
- If we start with the widest pair, what happens to width when either pointer moves inward? It always gets smaller.
- Which height limits the current area? The shorter wall.
- If we keep the shorter wall and move the taller one, can the limiting height increase? No; it remains the old shorter height or becomes even smaller, while width shrinks.
- Which wall should we replace to give ourselves a chance at a taller limiting height? The shorter one.

### Deriving the Greedy Move

Suppose `height[left] <= height[right]`. The current pair has area:

```text
(right - left) * height[left]
```

Keep `left` fixed and choose any new right index `right' < right`. Its width is smaller, and its limiting height cannot exceed `height[left]`. Therefore it cannot beat the current pair. The old `left` line is no longer useful, so move `left` inward. The symmetric argument applies when the right line is shorter. If they are equal, either pointer can move.

This discards pairs safely and checks only `O(N)` candidates instead of all `O(N^2)` pairs.

### Pointer Trace

```text
height = [1,8,6,2,5,4,8,3,7]
L=0, R=8: area = 8 * min(1,7) = 8   -> move L (shorter wall)
L=1, R=8: area = 7 * min(8,7) = 49 -> move R
L=1, R=7: area = 6 * min(8,3) = 18 -> move R
L=1, R=6: area = 5 * min(8,8) = 40 -> move either
...
Best area found = 49
```

---

## Layer 4 — C++ Implementation

The complete, compilable implementations are in `brute_force.cpp` and `optimized.cpp` in this folder.

### Brute Force

For every `left`, try every `right > left`, compute the area, and update the best answer.

- **Time:** `O(N^2)`, because there are about `N * (N - 1) / 2` pairs.
- **Extra space:** `O(1)`, because only a few integer variables are used.

### Optimized Two Pointers

Start with the widest pair. Record its area, then move the pointer at the shorter wall inward. Stop when the pointers meet.

- **Time:** `O(N)`, because each pointer moves inward at most `N - 1` times total.
- **Extra space:** `O(1)`, independent of the input size.

### Fast I/O Note

The standalone files include `ios_base::sync_with_stdio(false);` and `cin.tie(NULL);`. The first lets C++ streams run without synchronizing with C I/O; the second prevents `cin` from automatically flushing `cout` before every input operation. They are useful for programs with substantial input/output. These examples use fixed test cases, but keep the standard template consistent with the rest of this workspace.

### Edge Cases

- Two lines: only one possible container; the loop evaluates it once.
- Equal heights: either pointer may move; the maximum remains tracked.
- Zero-height lines: their area is zero, handled by the same formula.
- Strictly increasing or decreasing heights: still correct; the shorter side is moved each time.
- Large valid heights and widths: their product fits in `int` under the standard constraints.

---

## Layer 5 — Pattern Extraction

### Pattern: Opposite-End Two Pointers with a Greedy Discard

**Whenever** a problem asks you to choose two endpoints and the score depends on both their distance and a limiting endpoint value, ask whether the current pair is widest and whether one endpoint can be proven useless. Here, the shorter wall is the limiting factor, so discard it and search for a taller one.

### Reusable Template

```cpp
int left = 0;
int right = static_cast<int>(values.size()) - 1;
int best = 0;

while (left < right) {
    int score = (right - left) * min(values[left], values[right]);
    best = max(best, score);

    if (values[left] <= values[right]) {
        left++;
    } else {
        right--;
    }
}
```

**Must memorize:** The area formula and the proof for moving the shorter boundary.

**Re-derive each time:** Which pointer to move in a new problem. The move depends on that problem's score and needs its own correctness argument.

### Similar Problems to Try Next

| Problem                                   | What to practice                                                           | Link                                                                        |
| ----------------------------------------- | -------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| Two Sum II — Input Array Is Sorted (#167) | Opposite-end pointers and choosing a direction from the current result     | [LeetCode](https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/) |
| 3Sum (#15)                                | Sorting, fixing one value, and applying two pointers                       | [LeetCode](https://leetcode.com/problems/3sum/)                             |
| Trapping Rain Water (#42)                 | Boundary heights and water-level reasoning; a related but distinct problem | [LeetCode](https://leetcode.com/problems/trapping-rain-water/)              |
