# 📌 Add Two Numbers — Complete Notes

**LeetCode #2:** [Add Two Numbers](https://leetcode.com/problems/add-two-numbers/)
**Topic:** Linked List, Math, Recursion
**Difficulty:** 🟡 Medium
**Google-Tagged:** 🌟 Yes
**Phase:** 2 — Core Data Structures

---

## 🔵 LAYER 1 — What Is This Problem Asking?

### Problem Statement (Plain English)

You are given:

- Two **non-empty** linked lists
- Each list represents a **non-negative integer**
- The digits are stored in **reverse order** (least significant digit first)
- Each node contains a **single digit** (0–9)

Your job: Add the two numbers together and return the sum as a **new linked list**, also in reverse order.

### One-Line Rephrasing

> "Simulate elementary-school addition digit-by-digit on two reversed linked lists, handling carries."

### Example

```
Input:
  l1 = [2] → [4] → [3]        (represents 342)
  l2 = [5] → [6] → [4]        (represents 465)

Addition:
  342 + 465 = 807

Output:
  [7] → [0] → [8]             (represents 807)
```

Why reversed? Because addition works RIGHT to LEFT (ones → tens → hundreds), and having the ones digit FIRST means we can just traverse the list left to right. The reversal is a **gift** from the problem.

### Inputs & Outputs

```
INPUT:
  l1 → head of first linked list (non-empty, each node 0-9)
  l2 → head of second linked list (non-empty, each node 0-9)

OUTPUT:
  head of a new linked list representing l1 + l2
```

### Key Constraints & What They Imply

```
1 <= number of nodes <= 100     → lists can be different lengths
0 <= Node.val <= 9              → each node is a single digit
Numbers don't have leading zeros (except number 0 itself)
No negative numbers             → no sign handling needed
```

**Critical Implication:** Lists can be **different lengths** (e.g., 99 + 1 = 100). You must keep going until BOTH lists are exhausted AND carry is 0.

### ⚠️ Common Beginner Traps

1. **Forgetting the carry** — 5 + 7 = 12, you write 2 and carry 1. If you forget carry, you get wrong answers
2. **Stopping too early** — when one list ends, the other might still have digits left
3. **Missing the final carry** — 99 + 1 = 100. After both lists end, carry=1 creates an EXTRA node
4. **Converting to int then back** — lists can be 100 digits long. No integer type can hold that. You MUST do digit-by-digit
5. **Thinking you need to reverse** — digits are ALREADY in reverse order. That's the whole point

### Dry Run by Hand

```
l1 = [9] → [9] → [9]          (999)
l2 = [1]                       (1)

Step 1: 9+1     = 10 → write 0, carry 1
Step 2: 9+0+1   = 10 → write 0, carry 1   (l2 is exhausted, treat as 0)
Step 3: 9+0+1   = 10 → write 0, carry 1   (l2 is still 0)
Step 4: 0+0+1   =  1 → write 1, carry 0   (both lists done, but carry was 1)

Result: [0] → [0] → [0] → [1]  (1000) ✅
```

---

## 🟡 LAYER 2 — Core Concept: Linked Lists & Digit-by-Digit Addition

### What is a Linked List?

A linked list is a **chain of nodes** where each node contains:
- A **value** (data)
- A **pointer** to the next node

```
[val|next] → [val|next] → [val|next] → NULL
```

**Real-world analogy:**
Imagine a **treasure hunt** where each clue card has a message (value) and directions to the next clue (pointer). You MUST follow the chain — you can't jump to clue #5 directly. That's a linked list. No random access, only sequential.

### The ListNode Structure in C++

```cpp
struct ListNode {
    int val;           // the digit stored (0-9)
    ListNode* next;    // pointer to the next node (or NULL if last)

    // Constructors:
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};
```

**Key C++ concepts (first time seeing these):**
- `ListNode*` → a POINTER to a ListNode. It stores the memory ADDRESS of another node
- `->` → arrow operator. Used to access members through a pointer. `node->val` = "go to the node that this pointer points at, and get its val"
- `nullptr` → C++ way of saying "points to nothing" (end of list)

### Why This Problem Is Perfect for Linked Lists

You're doing **elementary school column addition**:
```
    3 4 2
  + 4 6 5
  -------
    8 0 7
```

Each "column" is one node. You process one column at a time, carrying over to the next. Linked lists let you build the result **one node at a time** as you go.

### The Dummy Head Trick

When building a new linked list, the first node is awkward — you have no "previous" node to link to. Solution: create a **dummy node** at the start, build after it, then return `dummy->next`.

```
dummy → [7] → [0] → [8] → NULL
         ↑
      return this (dummy->next)
```

This avoids ugly special-case code for the first node.

---

## 🟠 LAYER 3 — Problem-Solving Mindset

### Always Start With Brute Force

> Walk through both lists simultaneously. At each step, add the two digits plus any carry from the previous step. Build a new result node for each digit.

This problem is special: the **brute force IS the optimal solution**. There's no way to avoid visiting every digit — you need them all to compute the sum. The question is just: can you implement it cleanly?

### Ask Yourself (The Key Questions)

- *"Can I convert to int, add, and convert back?"*
  → NO. Numbers can be 100 digits long. Even `long long` only holds ~19 digits. You MUST simulate digit-by-digit.

- *"What happens when lists have different lengths?"*
  → Treat the shorter list's missing digits as 0. Keep going until BOTH lists are done.

- *"What about carry at the very end?"*
  → If carry > 0 after processing all digits, you need ONE more node.

- *"Do I need to reverse anything?"*
  → NO. The digits are ALREADY least-significant-first, which is exactly the order we need for addition.

### The Algorithm — Derive It Step by Step

```
l1 = [2] → [4] → [3]
l2 = [5] → [6] → [4]
carry = 0

Step 1:  sum = 2 + 5 + 0 = 7   → digit = 7%10 = 7,  carry = 7/10 = 0
         result: [7]

Step 2:  sum = 4 + 6 + 0 = 10  → digit = 10%10 = 0, carry = 10/10 = 1
         result: [7] → [0]

Step 3:  sum = 3 + 4 + 1 = 8   → digit = 8%10 = 8,  carry = 8/10 = 0
         result: [7] → [0] → [8]

Both lists done, carry = 0 → STOP.
Answer: [7] → [0] → [8]  ✅
```

### The Key Formula (Memorize This)

```
sum   = (l1 digit) + (l2 digit) + carry
digit = sum % 10       // what we write in this position
carry = sum / 10       // what we pass to the next position
```

This is literally how you add numbers by hand since grade school.

---

## 🔴 LAYER 4 — C++ Implementation

### Brute Force (= Optimal) — O(max(M,N)) time, O(max(M,N)) space

See brute_force.cpp and optimized.cpp

The iterative approach processes both lists simultaneously with a carry variable:

```cpp
ListNode* dummy = new ListNode(0);  // dummy head
ListNode* curr = dummy;             // current tail
int carry = 0;

while (l1 || l2 || carry) {
    int sum = carry;
    if (l1) { sum += l1->val; l1 = l1->next; }
    if (l2) { sum += l2->val; l2 = l2->next; }

    curr->next = new ListNode(sum % 10);
    curr = curr->next;
    carry = sum / 10;
}

return dummy->next;
```

### Why `while (l1 || l2 || carry)`?

Three conditions, each catches a different case:
- `l1` → first list still has digits
- `l2` → second list still has digits
- `carry` → e.g., 999 + 1 = 1000 — the final carry creates an extra node

If you only check `l1 || l2` you'll miss the final carry. This is the #1 bug.

### Recursive Approach

The problem also admits an elegant recursive formulation:

```cpp
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2, int carry = 0) {
    if (!l1 && !l2 && !carry) return nullptr;  // base case

    int sum = carry;
    if (l1) sum += l1->val;
    if (l2) sum += l2->val;

    ListNode* node = new ListNode(sum % 10);
    node->next = addTwoNumbers(
        l1 ? l1->next : nullptr,
        l2 ? l2->next : nullptr,
        sum / 10
    );
    return node;
}
```

Each recursive call processes one digit position, building the result list as the recursion unwinds.

### Complexity Table

| Approach  | Time         | Space        | Notes                        |
| --------- | ------------ | ------------ | ---------------------------- |
| Iterative | O(max(M,N))  | O(max(M,N))  | One pass, builds result list |
| Recursive | O(max(M,N))  | O(max(M,N))  | Same + recursion stack depth |

Where M = length of l1, N = length of l2.

**Why can't we do better?** We MUST read every digit of both numbers to compute the sum. O(max(M,N)) is the theoretical minimum.

### Edge Cases

```
[0] + [0]                  → [0]              (0 + 0 = 0)
[9,9,9] + [1]              → [0,0,0,1]        (999 + 1 = 1000, result longer than both inputs)
[9,9,9] + [9,9,9]          → [8,9,9,1]        (999 + 999 = 1998)
[1,0,0,0,0,0,...] + [5]    → unequal lengths, many trailing zeros in l1
```

---

## 🟣 LAYER 5 — Pattern Extraction

### Pattern Name: Linked List Digit-by-Digit Simulation

### The Rule

> "Whenever you process two linked lists in lockstep, use a dummy head, advance both pointers each step, and handle the case where one list runs out before the other."

### Trigger Words in a Problem

- "digits stored in a linked list" → digit-by-digit processing
- "add / multiply / compare two numbers as lists" → carry simulation
- "build a new linked list from computation" → dummy head pattern
- "reverse order of digits" → check if reversal helps or is already done

### Reusable Templates

**Template 1: Dummy Head for Building a New Linked List**

```cpp
ListNode* dummy = new ListNode(0);
ListNode* curr = dummy;

while (/* condition */) {
    curr->next = new ListNode(/* computed value */);
    curr = curr->next;
}

return dummy->next;  // skip the dummy
```

**Template 2: Process Two Lists Simultaneously**

```cpp
while (l1 || l2) {
    int v1 = l1 ? l1->val : 0;  // treat exhausted list as 0
    int v2 = l2 ? l2->val : 0;

    // ... process v1, v2 ...

    if (l1) l1 = l1->next;
    if (l2) l2 = l2->next;
}
```

### Must Memorize vs Re-derive

- **Memorize:** Dummy head trick — always use it when building a new linked list
- **Memorize:** `sum % 10` gives digit, `sum / 10` gives carry — this is universal
- **Memorize:** Loop condition includes `|| carry` for the trailing carry edge case
- **Re-derive:** Exact code structure — write it fresh each time from the pattern

### C++ Concepts Learned Here

- `struct ListNode` — defining a node with val + next pointer
- `new ListNode(x)` — dynamic memory allocation, creates a node on the heap
- `->` (arrow operator) — access member through a pointer (`node->val`, `node->next`)
- `nullptr` — null pointer literal (end of list / nothing)
- Ternary operator `l1 ? l1->val : 0` — compact null-safe value extraction

### Similar Problems to Attempt Next

| Problem                                 | Key Difference                        | Difficulty |
| --------------------------------------- | ------------------------------------- | ---------- |
| [Add Two Numbers II (LC 445)](https://leetcode.com/problems/add-two-numbers-ii/) | Digits in forward order → need stack/reverse | 🟡 Medium |
| [Merge Two Sorted Lists (LC 21)](https://leetcode.com/problems/merge-two-sorted-lists/) | Two-list traversal, no math           | 🟢 Easy   |
| [Multiply Strings (LC 43)](https://leetcode.com/problems/multiply-strings/) | Digit-by-digit multiplication         | 🟡 Medium |
| [Plus One (LC 66)](https://leetcode.com/problems/plus-one/) | Carry propagation on array            | 🟢 Easy   |
| [Reverse Linked List (LC 206)](https://leetcode.com/problems/reverse-linked-list/) | Fundamental linked list operation     | 🟢 Easy   |

---

*Phase 2 | Problem 01 | LeetCode #2 | Add Two Numbers*
