# 📌 Add Two Numbers — Complete Notes

**LeetCode #2:** [Add Two Numbers — LeetCode](https://leetcode.com/problems/add-two-numbers/)  
**Topic:** Linked List, Math, Recursion  
**Difficulty:** 🟡 Medium  
**Google-Tagged:** 🌟 Yes (Top-tier Google interview favorite)  
**Phase:** 2 — Core Data Structures  

---

## 🔵 LAYER 1 — What Is This Problem Asking?

### Problem Statement (Plain English)

You are given **two non-empty linked lists** representing two non-negative integers.
- The digits are stored in **reverse order** (meaning the 1's digit is at the head).
- Each node contains a **single digit** (`0` to `9`).
- Your task: **Add the two numbers** and return the sum as a **linked list** (also in reverse order).

### One-Line Rephrasing

> "Perform standard grade-school addition on two numbers digit-by-digit, starting from the ones place at the head, propagating the carry forward into a new linked list."

### Visual Example

```
Input:
l1:  2 ➔ 4 ➔ 3       (represents number 342)
l2:  5 ➔ 6 ➔ 4       (represents number 465)

Math:
    3 4 2
  + 4 6 5
  -------
    8 0 7

Output:
ans: 7 ➔ 0 ➔ 8       (represents number 807)
```

Explanation:
1. `2 + 5 = 7` (carry = 0) ➔ node `7`
2. `4 + 6 = 10` (sum = 0, carry = 1) ➔ node `0`
3. `3 + 4 + 1(carry) = 8` (carry = 0) ➔ node `8`

---

### Inputs & Outputs

```
INPUT:
  ListNode* l1  → head of first linked list (non-empty, values 0-9)
  ListNode* l2  → head of second linked list (non-empty, values 0-9)

OUTPUT:
  ListNode*     → head of the newly created sum linked list
```

---

### Key Constraints & What They Imply

```
1. Number of nodes in each list: [1, 100]
   🚨 CRITICAL IMPLICATION: A number can have up to 100 digits!
   Standard C++ primitive types:
     - `int` maxes out at ~2 * 10^9 (~10 digits)
     - `long long` maxes out at ~9 * 10^18 (~19 digits)
     - `__int128` maxes out at ~39 digits
   ❌ You CANNOT convert the lists to normal integer types. It WILL overflow!
   You MUST process the addition digit-by-digit (Big Integer simulation).

2. 0 <= Node.val <= 9
   → Each node contains strictly one base-10 digit.
   → Maximum sum at any column: 9 + 9 + 1 (previous carry) = 19.
   → Therefore, carry is always strictly either 0 or 1.

3. No leading zeros (except the number 0 itself)
   → Input numbers are clean canonical numbers (e.g., [0] represents 0, not [0, 0, 1]).
```

---

### ⚠️ Common Beginner Traps

1. **The Integer Overflow Trap (Fatal Mistake!):**
   Trying to turn `l1` into `int n1 = 342` and `l2` into `int n2 = 465`, adding them `n1 + n2`, and building a list. For 100 nodes, `long long` overflows immediately.
2. **Unequal List Lengths Trap:**
   Lists can have different lengths (e.g., `999` + `1`). If one list ends, you must continue processing the other list.
3. **The Lingering Carry Trap (Most Common Bug):**
   Consider `99 + 1 = 100`:
   `l1: 9 -> 9`
   `l2: 1`
   After both lists are fully traversed, there is still `carry = 1` remaining! You MUST create a final node `1`.
4. **Reverse Order Confusion:**
   Beginners often think "I should reverse the lists first!" **NO!** Reverse order is actually a huge gift. When you do addition on paper, you start from the right (ones place). Here, the head already points to the ones place!

---

### Dry Run by Hand

Let's trace `l1 = [9, 9]`, `l2 = [1]` (i.e., `99 + 1 = 100`):

| Step | Node `l1` | Node `l2` | Prev Carry | Calculation (`val1 + val2 + carry`) | New Digit (`sum % 10`) | Next Carry (`sum / 10`) | Result List Built So Far |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---|
| 1 | 9 | 1 | 0 | 9 + 1 + 0 = 10 | 0 | 1 | `7 ->` | `0` |
| 2 | 9 | null | 1 | 9 + 0 + 1 = 10 | 0 | 1 | `0 -> 0` |
| 3 | null | null | 1 | 0 + 0 + 1 = 1 | 1 | 0 | `0 -> 0 -> 1` |

Final Output: `0 -> 0 -> 1` (represents `100`).

---

## 🟡 LAYER 2 — Core Concept: Grade-School Math & Dummy Head

### 1. Grade-School Addition (Column Addition)

When adding two large numbers by hand:
```
    Carries:  1  1
             9  9  9
          +     1  5
          ----------
          1  0  1  4
```
At every single column:
- $\text{sum} = \text{digit}_1 + \text{digit}_2 + \text{carry}$
- $\text{new\_digit} = \text{sum} \pmod{10}$
- $\text{new\_carry} = \lfloor \text{sum} / 10 \rfloor$

Since our linked list puts the lowest digit at the **head**, we traverse left-to-right through the nodes, applying this exact formula.

---

### 2. The "Dummy Head" Pattern (A Must-Know for Linked Lists)

#### The Problem Without Dummy Head:
When creating a brand new linked list, the first node is a special case:
```cpp
ListNode* head = nullptr;
ListNode* tail = nullptr;

// For every new node:
if (head == nullptr) {
    head = new ListNode(digit);
    tail = head;
} else {
    tail->next = new ListNode(digit);
    tail = tail->next;
}
```
Notice how every single iteration has to check `if (head == nullptr)`? This adds messy boilerplate and introduces pointer bugs.

#### The Solution: The Dummy Node!
Create a fake starting node on the heap or stack:
```
[Dummy: 0] ➔ nullptr
     ▲
     └── tail
```
Every time a new node is created, you simply attach it:
```cpp
tail->next = new ListNode(digit);
tail = tail->next;
```
When finished, the real answer begins at:
```cpp
ListNode* result = dummy->next;
```
The dummy node eliminates all special cases for the first node!

---

## 🟠 LAYER 3 — Problem-Solving Mindset

### The Inner Monologue (How to Arrive at the Solution)

1. *"Can I convert both lists to numbers, add them, and convert back?"*
   - Let's check constraints: Length up to 100 digits.
   - 100 digits will overflow `int` (10 digits) and `long long` (19 digits).
   - Conclusion: **Impossible with standard numeric types.**

2. *"How do I add numbers that don't fit in standard variables?"*
   - Brute force / naive: Extract all digits into two `vector<int>` or `string`, perform school addition in a loop to produce a result `vector<int>`, then convert that vector into a linked list.
   - Let's analyze: That works! But it requires $O(N)$ extra space to store intermediate vectors and takes two separate passes (one to extract, one to build).

3. *"Can I do this in a single pass without intermediate vectors?"*
   - Look at the lists: `l1` has digit 0, `l2` has digit 0.
   - I can add `l1->val + l2->val` directly right now, calculate the new digit, attach a new node, and advance pointers!
   - We only need one auxiliary integer: `carry`.

4. *"What happens if one list is shorter than the other?"*
   - If `l1` is null, its contribution to the sum is simply `0`.
   - `int val1 = (l1 != nullptr) ? l1->val : 0;`

5. *"When does the loop terminate?"*
   - If `l1` runs out, we might still have `l2`.
   - If both run out, we might still have a `carry == 1`.
   - So keep going as long as:
     `while (l1 != nullptr || l2 != nullptr || carry != 0)`

---

## 🔴 LAYER 4 — C++ Implementation

### Approach 1: Brute Force (Vector Extraction & Addition)
*(See [brute_force.cpp](file:///c:/Users/kunal/Desktop/DSA%20self%20practise/Phase_2_Core_DS/01_add_two_numbers_2/brute_force.cpp))*

Extract node values into arrays, simulate column addition, then allocate nodes:
```cpp
// 1. Extract values into std::vector<int>
vector<int> v1, v2;
while (l1) { v1.push_back(l1->val); l1 = l1->next; }
while (l2) { v2.push_back(l2->val); l2 = l2->next; }

// 2. Perform digit addition into v_sum
vector<int> v_sum;
int i = 0, j = 0, carry = 0;
while (i < v1.size() || j < v2.size() || carry) {
    int val1 = (i < v1.size()) ? v1[i++] : 0;
    int val2 = (j < v2.size()) ? v2[j++] : 0;
    int sum = val1 + val2 + carry;
    v_sum.push_back(sum % 10);
    carry = sum / 10;
}

// 3. Build resulting linked list from v_sum
ListNode dummy(0);
ListNode* curr = &dummy;
for (int d : v_sum) {
    curr->next = new ListNode(d);
    curr = curr->next;
}
return dummy.next;
```

---

### Approach 2: Optimal (One-Pass Direct Simulation with Dummy Head)
*(See [optimized.cpp](file:///c:/Users/kunal/Desktop/DSA%20self%20practise/Phase_2_Core_DS/01_add_two_numbers_2/optimized.cpp))*

```cpp
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode* dummy = new ListNode(0); // Dummy sentinel node
    ListNode* curr = dummy;            // Traversal pointer
    int carry = 0;

    // Loop continues as long as there is at least one digit or a carry left
    while (l1 != nullptr || l2 != nullptr || carry != 0) {
        int val1 = (l1 != nullptr) ? l1->val : 0;
        int val2 = (l2 != nullptr) ? l2->val : 0;

        int sum = val1 + val2 + carry;
        carry = sum / 10; // New carry for next iteration (0 or 1)

        curr->next = new ListNode(sum % 10); // Store single-digit value
        curr = curr->next;

        // Advance source list pointers if available
        if (l1 != nullptr) l1 = l1->next;
        if (l2 != nullptr) l2 = l2->next;
    }

    ListNode* result = dummy->next;
    delete dummy; // Clean up the dummy node to avoid memory leaks
    return result;
}
```

---

### Complexity Comparison

| Metric | Brute Force (Vector Extraction) | Optimized (One-Pass Traversal) |
|---|---|---|
| **Time Complexity** | $O(\max(N, M))$ (3 distinct passes) | $O(\max(N, M))$ (Single pass) |
| **Auxiliary Space** | $O(N + M)$ (Stores digits in vectors) | $O(1)$ (Only pointers & `carry` variable) |
| **Output Space** | $O(\max(N, M))$ for output nodes | $O(\max(N, M))$ for output nodes |
| **Operations for $N=100$** | ~300 operations + dynamic heap allocations | ~100 direct operations |

---

### Edge Cases Checked

1. **Different lengths:** `l1 = [9, 9]`, `l2 = [1]` ➔ `[0, 0, 1]` (handles trailing carry + length mismatch)
2. **Zeros:** `l1 = [0]`, `l2 = [0]` ➔ `[0]`
3. **Carry throughout all digits:** `[9, 9, 9] + [1]` ➔ `[0, 0, 0, 1]` (chain carry propagation)
4. **Single-digit carry:** `[5] + [5]` ➔ `[0, 1]`

---

## 🟣 LAYER 5 — Pattern Extraction

### Pattern Name: Simultaneous Multi-List Traversal with Sentinel (Dummy) Node

### The Fundamental Rule

> **Rule 1 (Linked List Construction):**  
> "Whenever you need to build a new linked list dynamically, always create a **Dummy Head** sentinel. Attach new nodes to `curr->next`, then return `dummy->next`."

> **Rule 2 (Digit Math & Simulation):**  
> "Whenever you simulate column-by-column math across two streams (lists, strings, or arrays), keep the loop condition running while **any** stream has elements **OR** the `carry > 0`."

### Trigger Words in Problem Statements

- *"Add two numbers represented as linked lists / strings"*
- *"Digits are stored in reverse order"*
- *"Merge two sorted lists / combine two lists into a new one"*
- *"Arbitrary-precision arithmetic / BigInt simulation"*

---

### Reusable Template

```cpp
// Generic Two-Pointer Stream Addition Template
ListNode* dummy = new ListNode(0);
ListNode* curr = dummy;
int carry = 0;

while (p1 != nullptr || p2 != nullptr || carry != 0) {
    int v1 = p1 ? p1->val : 0;
    int v2 = p2 ? p2->val : 0;

    int total = v1 + v2 + carry;
    carry = total / BASE; // BASE = 10 for decimal, 2 for binary

    curr->next = new ListNode(total % BASE);
    curr = curr->next;

    if (p1) p1 = p1->next;
    if (p2) p2 = p2->next;
}

ListNode* head = dummy->next;
delete dummy;
return head;
```

---

### Must Memorize vs Re-derive

- **Must Memorize:**
  1. Sentinel / Dummy Head idiom (`ListNode* dummy = new ListNode(0); ListNode* curr = dummy; return dummy->next;`)
  2. The compound loop guard: `while (l1 || l2 || carry)` (prevents forgetting the trailing carry!)
- **Re-derive Each Time:**
  - Base math logic (`sum / 10`, `sum % 10`).

---

### C++ Concepts Learned Here

1. **Custom Structs & Constructors:**
   ```cpp
   struct ListNode {
       int val;
       ListNode *next;
       ListNode(int x) : val(x), next(nullptr) {}
   };
   ```
2. **Safe Pointer Access with Ternary Operator:**
   `int val = (l1 != nullptr) ? l1->val : 0;` prevents dereferencing null pointers.
3. **Dynamic Memory & Deletion (`delete`):**
   When `dummy` is allocated with `new ListNode(0)`, we delete it before returning `dummy->next` to prevent memory leaks in production C++.

---

### Similar Problems to Attempt Next

| Problem | Key Difference | Difficulty |
|---|---|:---:|
| [Add Two Numbers II (LC 445)](https://leetcode.com/problems/add-two-numbers-ii/) | Digits are in **most significant first** order ➔ Use Stacks or Reverse Lists | 🟡 Medium |
| [Add Strings (LC 415)](https://leetcode.com/problems/add-strings/) | Same column addition math on strings instead of linked lists | 🟢 Easy |
| [Add Binary (LC 67)](https://leetcode.com/problems/add-binary/) | Base 2 instead of Base 10 (`sum % 2`, `carry = sum / 2`) | 🟢 Easy |
| [Plus One (LC 66)](https://leetcode.com/problems/plus-one/) | Adding 1 to an array of digits | 🟢 Easy |
| [Merge Two Sorted Lists (LC 21)](https://leetcode.com/problems/merge-two-sorted-lists/) | Uses the exact same Dummy Head pattern to merge two lists | 🟢 Easy |

---

*Phase 2 | Problem 01 | LeetCode #2 | Add Two Numbers*
