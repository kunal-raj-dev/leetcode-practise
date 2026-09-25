# 📌 Two Sum — Complete Notes

**LeetCode #1:** [Two S](https://leetcode.com/problems/two-sum/)[github.com/kunal-raj-dev/leetcode-practise.git](https://github.com/kunal-raj-dev/leetcode-practise.git)um — LeetCode
**Topic:** Array, Hash Table
**Difficulty:** 🟢 Easy
**Google-Tagged:** 🌟 Yes
**Phase:** 1 — Foundation

---

## 🔵 LAYER 1 — What Is This Problem Asking?

### Problem Statement (Plain English)

You are given:

- A list of numbers (called `nums`)
- A single number called `target`

Your job: Find **two numbers** in the list that **add up to target**.
Then return their **positions (indices)** — not the numbers themselves.

### One-Line Rephrasing

> "Find the two positions in the array whose values sum to target."

### Example

```
nums   = [2, 7, 11, 15]
target = 9

Position 0 → value 2
Position 1 → value 7
2 + 7 = 9 ✅

Answer: [0, 1]
```

### Inputs & Outputs

```
INPUT:
  nums[]  → array of integers (can be negative, zero, large)
  target  → a single integer

OUTPUT:
  [i, j] such that nums[i] + nums[j] == target
```

### Key Constraints & What They Imply

```
2 <= nums.length <= 10^4       → at least 2 elements, at most 10,000
-10^9 <= nums[i] <= 10^9      → values can be very large or very negative
Exactly ONE valid answer       → don't worry about "no solution" case
Cannot use same element twice  → i != j always
```

### ⚠️ Common Beginner Traps

1. **Returning values instead of indices** — problem asks for POSITIONS
2. **Using same element twice** — nums[i]+nums[i] NOT allowed even if it equals target
3. **Assuming array is sorted** — it is NOT sorted
4. **Overthinking multiple answers** — exactly one answer guaranteed

### Dry Run by Hand

```
nums = [3, 2, 4], target = 6

Pairs:
  (3, 2) → 3+2 = 5 ❌
  (3, 4) → 3+4 = 7 ❌
  (2, 4) → 2+4 = 6 ✅ → indices [1, 2]

Answer: [1, 2]
```

---

## 🟡 LAYER 2 — Core Concept: Hash Map

### What is a Hash Map?

Think of it as a **magic dictionary**:

- Store a KEY → VALUE pair
- Look up any key **instantly** in O(1) — no searching

**Real-world analogy:**
A locker room where each locker has a number (key) and stores a name (value).
To find "who is in locker 42?" you go directly to locker 42.
You don't check locker 1, 2, 3... That's O(1). That's a hash map.

### In C++ — unordered_map

```cpp
unordered_map<int, int> mp;

mp[7] = 0;        // store: key=7, value=0
mp.count(7);      // → 1 if key 7 exists, 0 if not
mp[7];            // → retrieves value at key 7 → gives 0
```

### Why Hash Map Fits This Problem

For each nums[i], the question becomes:

> "Has (target - nums[i]) appeared before in the array?"

Storing seen numbers in a hash map lets us answer that in O(1).
This turns an O(N^2) search into a single O(N) pass.

---

## 🟠 LAYER 3 — Problem-Solving Mindset

### Always Start With Brute Force

> Check every pair. If they sum to target, return their indices.

```
For every i:
  For every j > i:
    if nums[i] + nums[j] == target → return [i, j]
```

This is O(N^2). Correct, but slow.

### Ask Yourself (The Key Questions)

- *"What am I doing repeatedly?"*
  → For each element, I'm scanning for its complement. Repeated scan = optimize!
- *"Can I make that search O(1) instead of O(N)?"*
  → YES — store seen elements in a hash map.

### The Insight — Derive It Step by Step

```
nums = [2, 7, 11, 15],  target = 9

i=0: num=2, complement=9-2=7
     Is 7 in map? NO → store {2:0}

i=1: num=7, complement=9-7=2
     Is 2 in map? YES! at index 0
     → return [0, 1] ✅
```

Found in one pass. Never needed to check 11 or 15.

---

## 🔴 LAYER 4 — C++ Implementation

### Brute Force — O(N^2) time, O(1) space

See brute_force.cpp

```cpp
for (int i = 0; i < n; i++)
    for (int j = i+1; j < n; j++)
        if (nums[i] + nums[j] == target)
            return {i, j};
```

### Optimized — O(N) time, O(N) space

See optimized.cpp

```cpp
unordered_map<int, int> mp;
for (int i = 0; i < n; i++) {
    int complement = target - nums[i];
    if (mp.count(complement))
        return {mp[complement], i};
    mp[nums[i]] = i; // store AFTER checking — prevents self-match
}
```

### ❓ Why Store AFTER Checking?

```
nums = [3, 3], target = 6

i=0: complement=3. Is 3 in map? NO (empty). Store {3:0}
i=1: complement=3. Is 3 in map? YES at index 0.
     Return [0, 1] ✅ — different indices, correct!

If we stored BEFORE checking:
i=0: Store {3:0}. Is 3 in map? YES (just stored it!). Return [0,0] ❌ WRONG
```

### Complexity Table

| Approach    | Time   | Space | For N=10,000     |
| ----------- | ------ | ----- | ---------------- |
| Brute Force | O(N^2) | O(1)  | ~100M operations |
| Hash Map    | O(N)   | O(N)  | ~10K operations  |

### Edge Cases

```
[3, 3],     target=6   → same values, diff indices → [0,1] ✅
[-1, -2],   target=-3  → negatives work fine       → [0,1] ✅
[0, 4, 3, 0], target=0 → zeros work fine           → [0,3] ✅
[1, 2],     target=3   → minimum input (2 elements) → [0,1] ✅
```

---

## 🟣 LAYER 5 — Pattern Extraction

### Pattern Name: Complement Search with Hash Map

### The Rule

> "Whenever you need TWO elements that satisfy a sum condition
> → compute complement = target - current
> → use hash map for O(1) lookup of complement"

### Trigger Words in a Problem

- "find two numbers that add to X" → Hash Map + Complement
- "find a pair satisfying [condition]" → Hash Map or Two Pointers
- "have you seen this value before?" → Hash Set or Hash Map

### Reusable Template

```cpp
unordered_map<int, int> mp; // {value → index}
for (int i = 0; i < n; i++) {
    int complement = target - nums[i];
    if (mp.count(complement))
        return {mp[complement], i};
    mp[nums[i]] = i; // store AFTER checking
}
```

### Must Memorize vs Re-derive

- **Memorize:** The PATTERN — complement → hash map
- **Memorize:** Store AFTER checking (prevents self-match)
- **Re-derive:** Exact code each time (never memorize code)

### C++ Concepts Learned Here

- `unordered_map<K,V>` — O(1) average insert/lookup
- `mp.count(key)` — check existence (returns 0 or 1)
- `mp[key]` — access value at key
- Return `{a, b}` for a vector — initializer list syntax

### Similar Problems to Attempt Next

| Problem                        | Key Difference               | Difficulty |
| ------------------------------ | ---------------------------- | ---------- |
| Contains Duplicate (LC 217)    | Hash Set, not map            | 🟢 Easy    |
| Valid Anagram (LC 242)         | Frequency count              | 🟢 Easy    |
| Two Sum II (LC 167)            | Sorted array → Two Pointers | 🟢 Easy    |
| 3Sum (LC 15)                   | Extend to 3 elements         | 🟡 Medium  |
| Subarray Sum Equals K (LC 560) | Prefix sum + hash map        | 🟡 Medium  |

---

*Phase 1 | Problem 01 | LeetCode #1 | Two Sum*
