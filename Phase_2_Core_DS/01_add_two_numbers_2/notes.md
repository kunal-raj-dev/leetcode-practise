# 🔗 Add Two Numbers

**LeetCode Link:** https://leetcode.com/problems/add-two-numbers

---

## 🏷️ Session Tag

```
Problem    : Add Two Numbers
Topic      : Linked List, Math, Recursion
Difficulty : Medium
Google-Tagged: Yes
Phase      : 2 (Core Data Structures)
```

---

## 🔵 LAYER 1 — Problem Deconstruction ("What is this even asking?")

### Plain English Explanation

Imagine you have two numbers — say **342** and **465**.  
But instead of being stored normally, each number is stored as a **Linked List** — where each node holds **one digit**, and importantly, the digits are stored **in reverse order** (least significant digit first).

So:
- 342 → stored as: `2 → 4 → 3`
- 465 → stored as: `5 → 6 → 4`

You need to **add these two numbers** and return the **result also as a linked list** (in the same reverse order).

Expected Output for the above: `342 + 465 = 807` → returned as `7 → 0 → 8`

---

### Inputs & Outputs

| | Detail |
|---|---|
| **Input** | Two non-empty linked lists `l1` and `l2` (digits in reverse order) |
| **Output** | A new linked list representing the sum (digits in reverse order) |

---

### Constraints (and what they mean for us)

| Constraint | Implication |
|---|---|
| Each node holds a digit `[0,9]` | No weird values, just single digits |
| No leading zeros (except `0` itself) | Input is clean |
| Lists can have different lengths | We must handle one list ending before the other |
| Sum can overflow `int` or `long long` | Irrelevant — we work digit by digit, not as a full number |

---

### Dry Run by Hand — Small Example

```
l1: 2 → 4 → 3    (represents 342)
l2: 5 → 6 → 4    (represents 465)

Step 1: 2 + 5 = 7,  carry = 0  → node: 7
Step 2: 4 + 6 = 10, carry = 1  → node: 0
Step 3: 3 + 4 + 1(carry) = 8, carry = 0  → node: 8

Result: 7 → 0 → 8   (represents 807) ✅
```

---

### Common Traps & Misreadings

1. **Forgetting carry at the end:** If after both lists are done, carry is still `1`, you MUST add one more node.
   - Example: `9 → 9` + `1` = `0 → 0 → 1` (i.e., `99 + 1 = 100`)
2. **Thinking you need to reverse the lists first:** You DON'T. The reverse order is actually a gift — it means we naturally add from least significant digit, which is how real addition works.
3. **Forgetting to handle lists of different lengths:** One list may be longer. When one ends, treat missing nodes as `0`.
4. **Memory leak / dangling pointers:** You're creating new nodes — keep track of your head pointer.

---

### One-Sentence Summary

> "We simulate grade-school digit-by-digit addition on two reversed linked lists, carrying over the remainder, and build a new reversed linked list as the result."

---

## 🟡 LAYER 2 — Concept Building ("What world does this problem live in?")

### DS/Algo Family

This problem lives in **Linked Lists + Math Simulation**.

It is **not** about finding a pattern or applying a clever algorithm.  
It is about carefully **simulating** the process of addition, handling all the edge cases.

---

### Why Linked Lists Fit Here

Think of a linked list like a **train**:
- Each carriage (node) holds one digit
- The train can be any length
- You can only visit one carriage at a time, left to right

In this problem:
- We walk both trains simultaneously, one node at a time
- At each stop, we add the two digits + any carry from the previous stop
- We build a new train (result list) as we go

---

### Visual Explanation — ASCII Art

```
l1:  [2] → [4] → [3] → NULL
l2:  [5] → [6] → [4] → NULL
              carry: 0

Pass 1:       2 + 5 + 0(carry) = 7,   new_carry = 0
              result: [7]

Pass 2:       4 + 6 + 0(carry) = 10,  new_carry = 1
              result: [7] → [0]

Pass 3:       3 + 4 + 1(carry) = 8,   new_carry = 0
              result: [7] → [0] → [8]

Both lists done, carry = 0 → STOP
Final: [7] → [0] → [8] → NULL
```

---

### Real-World Analogy

Think of two people adding numbers by hand in a notebook:
- Person A reads digits of the first number one by one
- Person B reads digits of the second number one by one
- A third person writes down the result, and holds a "carry slip" when the sum exceeds 9

The carry slip is passed forward to the next column. That's exactly what we do in code.

---

### Prerequisite Theory — Linked Lists in C++

If you haven't used linked lists before, here's the minimum you need:

```cpp
// A node in a linked list
struct ListNode {
    int val;        // the digit stored
    ListNode* next; // pointer to the next node

    // Constructor
    ListNode(int x) : val(x), next(nullptr) {}
};
```

**Key operations:**
```cpp
ListNode* curr = head;     // start from the head node
curr->val;                 // access the digit
curr = curr->next;         // move to the next node
curr == nullptr;           // means we've reached the end of the list
```

**Building a new list using a dummy head (critical technique):**
```cpp
// Dummy node trick — avoids special-casing the first node
ListNode dummy(0);
ListNode* tail = &dummy;   // tail always points to the last node we added

// To add a new node:
tail->next = new ListNode(digit);
tail = tail->next;

// At the end, the real result starts at:
return dummy.next;
```

> **Why dummy head?** Without it, you'd need an `if` statement to handle the very first node specially. With it, every node addition is uniform — no special case.

---

## 🟠 LAYER 3 — Problem-Solving Mindset ("How do I think my way to a solution?")

> Always start with brute force. Never skip it.

### Inner Monologue — Walk Through Your Thinking

**Question 1:** What am I doing repeatedly?
→ "I'm visiting one node from each list and adding their values."
→ **Signal: Use a loop that processes both lists together.**

**Question 2:** What extra state do I need to carry between steps?
→ "The carry from the previous column's addition."
→ **Signal: Maintain a `carry` variable across iterations.**

**Question 3:** What happens when one list is shorter?
→ "I should treat missing nodes as having digit `0`."
→ **Signal: Use `(l1 ? l1->val : 0)` style ternary expressions.**

**Question 4:** When do I stop?
→ "When BOTH lists are exhausted AND carry is 0."
→ **Signal: Loop condition is `while (l1 || l2 || carry)`.**

---

### Brute Force Approach — Extract Numbers, Add, Rebuild

**Idea:** Convert both linked lists to actual numbers, add them, convert the sum back to a linked list.

**Why this fails:**
- The numbers can have **up to 100 digits** each (per constraints)
- `int`, `long long` — all overflow with 100-digit numbers
- This approach breaks at scale

**Conclusion:** We CANNOT use this. But understanding why helps us appreciate the correct approach.

---

### Optimal Approach — Digit-by-Digit Simulation

**Idea:** Don't reconstruct the numbers. Instead, walk both lists simultaneously, add digit by digit, manage carry, build result list on the fly.

This is like performing long addition by hand — the exact real-world way numbers are added.

**Loop condition:** `while (l1 != nullptr || l2 != nullptr || carry != 0)`

**At each iteration:**
1. Get digit from l1 (or 0 if l1 is exhausted)
2. Get digit from l2 (or 0 if l2 is exhausted)
3. `sum = d1 + d2 + carry`
4. `carry = sum / 10`  (will be 0 or 1)
5. `digit = sum % 10`  (the digit to store in result)
6. Create a new node with `digit`, attach it to result list
7. Advance l1 and l2 if they're not null

---

## 🔴 LAYER 4 — Implementation ("Let's write the C++ code")

### BRUTE FORCE — Why we skip it properly

As discussed, extracting numbers doesn't work for large inputs (100 digits = overflow).  
The "brute force" here conceptually is the simulation — there's no inferior starting point to demonstrate.  
Instead, we show a slightly-less-clean version first, then refine.

**See:** [`brute_force.cpp`](./brute_force.cpp)  
(This is the "naive simulation" — correct but without the dummy head trick, harder to read)

---

### OPTIMIZED — Dummy Head Simulation

**See:** [`optimized.cpp`](./optimized.cpp)

**Core logic walkthrough:**

```cpp
int sum   = d1 + d2 + carry;  // total at this digit position
carry     = sum / 10;          // 0 or 1 — carried to next position
int digit = sum % 10;          // actual digit to store
```

**Why `sum / 10` gives carry:**
- If sum = 7: carry = 0, digit = 7
- If sum = 10: carry = 1, digit = 0
- If sum = 17: carry = 1, digit = 7
- If sum = 18: carry = 1, digit = 8
- Maximum possible sum = 9 + 9 + 1(carry) = 19 → carry is always 0 or 1

---

### Complexity Analysis

**Time Complexity: O(max(M, N))**
- M = length of l1, N = length of l2
- We visit every node exactly once
- The extra carry check at the end is O(1)
- **Intuition:** We do one unit of work per digit of the longer number

**Space Complexity: O(max(M, N) + 1)**
- We create a new result list with at most `max(M, N) + 1` nodes
- The `+1` is for the possible extra carry node (e.g., 99 + 1 = 100, three digits from two)
- **Intuition:** We always allocate exactly as many result nodes as the result has digits
- We often write this as **O(max(M, N))** dropping the constant

---

### Edge Cases — Verified ✅

| Edge Case | Input | Expected Output |
|---|---|---|
| Different length lists | `[2,4,3]` + `[5,6]` | `[7,0,4]` (342 + 65 = 407) |
| Carry at end | `[9,9]` + `[1]` | `[0,0,1]` (99 + 1 = 100) |
| Both single digit, no carry | `[2]` + `[3]` | `[5]` |
| Both single digit, with carry | `[5]` + `[5]` | `[0,1]` (5 + 5 = 10) |
| One list is zero | `[0]` + `[0]` | `[0]` |
| Max carry propagation | `[9,9,9]` + `[9,9,9]` | `[8,9,9,1]` (999+999=1998) |

---

## 🟣 LAYER 5 — Pattern Extraction ("What did we actually learn?")

### The Reusable Template

```
Whenever you see: Two linked lists, simulate a process node by node
Think:
    1. Use while (l1 || l2 || extra_state) as your loop
    2. Use ternary: val = (ptr ? ptr->val : 0) to handle different lengths
    3. Use dummy head to cleanly build a result list
    4. Advance pointers carefully: if (ptr) ptr = ptr->next
```

### "Whenever You See X, Think Y"

| Trigger Pattern | Your Reflex |
|---|---|
| "Add/merge two linked lists" | Dual-pointer traversal with dummy head |
| "Carry/remainder propagates" | `carry = sum / 10`, `digit = sum % 10` |
| "Lists of different lengths" | Treat missing as `0`, loop until both exhausted |
| "Build result as linked list" | Dummy head trick — avoid first-node special case |

### Must Memorize vs Re-derive

| Concept | Status |
|---|---|
| `carry = sum / 10`, `digit = sum % 10` | ⭐ **Must Memorize** |
| Dummy head trick for building lists | ⭐ **Must Memorize** |
| Loop condition `while(l1 \|\| l2 \|\| carry)` | ⭐ **Must Memorize** |
| The simulation logic itself | ✅ Re-derive each time |

---

### Similar Problems to Attempt Next

| # | Problem | LeetCode | Connection |
|---|---|---|---|
| 1 | Multiply Strings | [LC 43](https://leetcode.com/problems/multiply-strings/) | Digit-by-digit simulation |
| 2 | Add Binary | [LC 67](https://leetcode.com/problems/add-binary/) | Same carry logic, on strings |
| 3 | Merge Two Sorted Lists | [LC 21](https://leetcode.com/problems/merge-two-sorted-lists/) | Same dual-pointer + dummy head technique |
| 4 | Reverse Linked List | [LC 206](https://leetcode.com/problems/reverse-linked-list/) | Core linked list manipulation |
| 5 | Add Two Numbers II | [LC 445](https://leetcode.com/problems/add-two-numbers-ii/) | Same problem but digits in forward order — uses a Stack! |

---

### Recursion Approach (Bonus — mentioned in problem tags)

The iterative approach is preferred in interviews, but recursion is elegant:

```
addTwoNumbers(l1, l2, carry):
    if l1 == null AND l2 == null AND carry == 0:
        return null
    
    sum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + carry
    
    node = new ListNode(sum % 10)
    node->next = addTwoNumbers(next of l1, next of l2, sum / 10)
    
    return node
```

Each recursive call handles one digit position. The recursion stack grows to O(max(M,N)) depth.

---

*Last Updated: 2026-09-25 | Phase: 2 | Language: C++*
