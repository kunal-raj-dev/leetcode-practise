// ============================================================
// Problem : Add Two Numbers (LeetCode #2)
// Link    : https://leetcode.com/problems/add-two-numbers
// Approach: OPTIMIZED — Digit-by-digit simulation WITH dummy head
// Time    : O(max(M, N))
// Space   : O(max(M, N))
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// Definition for singly-linked list.
// Provided by LeetCode — don't redefine in actual submission.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {

        // ── DUMMY HEAD TRICK ─────────────────────────────────────────
        // We create a fake "sentinel" node at the start of our result list.
        // Its value doesn't matter (we use 0). It's just a placeholder.
        //
        // WHY?
        // Without it, we need to special-case the very first result node:
        //    if (head == nullptr) { head = newNode; tail = newNode; }
        //    else { tail->next = newNode; tail = tail->next; }
        //
        // With a dummy head, EVERY new node is added the same way:
        //    tail->next = new ListNode(digit);
        //    tail = tail->next;
        //
        // At the end, the real result starts at dummy.next (skipping the placeholder).
        // ─────────────────────────────────────────────────────────────
        ListNode dummy(0);         // placeholder (sentinel) node — value unused
        ListNode* tail = &dummy;   // tail always points to the last node we added

        int carry = 0; // carry from previous digit addition (0 or 1)

        // ── MAIN LOOP ────────────────────────────────────────────────
        // Continue as long as there are digits left in EITHER list,
        // OR there's a carry remaining.
        //
        // KEY: We use || (OR) not && (AND)
        //   If we used &&, we'd stop when the shorter list ends.
        //   But the longer list might still have digits we haven't processed!
        // ─────────────────────────────────────────────────────────────
        while (l1 != nullptr || l2 != nullptr || carry != 0) {

            // Get the current digit from l1.
            // If l1 is exhausted (nullptr), treat it as digit 0.
            // This handles the case where l1 is shorter than l2.
            int d1 = (l1 != nullptr) ? l1->val : 0;

            // Get the current digit from l2.
            // Same idea — treat exhausted list as contributing 0.
            int d2 = (l2 != nullptr) ? l2->val : 0;

            // Sum = both digits + carry from the previous position
            // Maximum possible: 9 + 9 + 1(carry) = 19 → carry will be at most 1
            int sum = d1 + d2 + carry;

            // Compute the new carry (0 if sum < 10, 1 if sum >= 10)
            // Integer division: 7/10 = 0, 10/10 = 1, 17/10 = 1, 19/10 = 1
            carry = sum / 10;

            // Compute the actual digit to store at this position
            // Modulo: 7%10 = 7, 10%10 = 0, 17%10 = 7, 19%10 = 9
            int digit = sum % 10;

            // Create a new node with this digit and attach it to the result list.
            // Because we have a dummy head, this works uniformly for ALL nodes —
            // no special case for the first node.
            tail->next = new ListNode(digit);
            tail = tail->next; // advance tail to the newly added node

            // Advance l1 and l2 to their next nodes (only if not already null)
            if (l1 != nullptr) l1 = l1->next;
            if (l2 != nullptr) l2 = l2->next;
        }

        // dummy.next is the actual first node of the result list
        // (dummy itself is just the placeholder we never included in results)
        return dummy.next;
    }
};

// ============================================================
// BONUS — RECURSIVE APPROACH
// Elegant, but watch the call stack for very long lists (100 nodes deep).
// ============================================================

class SolutionRecursive {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        return solve(l1, l2, 0);
    }

private:
    ListNode* solve(ListNode* l1, ListNode* l2, int carry) {
        // Base case: both lists done and no carry left — nothing to return
        if (!l1 && !l2 && carry == 0) return nullptr;

        int d1 = l1 ? l1->val : 0;
        int d2 = l2 ? l2->val : 0;
        int sum = d1 + d2 + carry;

        // Create node for this digit position
        ListNode* node = new ListNode(sum % 10);

        // Recursively compute the rest (next digit positions)
        // Pass the new carry forward
        node->next = solve(
            l1 ? l1->next : nullptr,
            l2 ? l2->next : nullptr,
            sum / 10
        );

        return node;
    }
};

// ============================================================
// TEST HELPERS
// ============================================================

ListNode* buildList(vector<int> digits) {
    if (digits.empty()) return nullptr;
    ListNode* head = new ListNode(digits[0]);
    ListNode* curr = head;
    for (int i = 1; i < (int)digits.size(); i++) {
        curr->next = new ListNode(digits[i]);
        curr = curr->next;
    }
    return head;
}

void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " → ";
        head = head->next;
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    Solution sol;

    // Test 1: 342 + 465 = 807    → [7, 0, 8]
    cout << "Test 1 (342 + 465 = 807):    ";
    printList(sol.addTwoNumbers(buildList({2, 4, 3}), buildList({5, 6, 4})));

    // Test 2: 99 + 1 = 100       → [0, 0, 1]   (carry at the end!)
    cout << "Test 2 (99 + 1 = 100):       ";
    printList(sol.addTwoNumbers(buildList({9, 9}), buildList({1})));

    // Test 3: 0 + 0 = 0          → [0]
    cout << "Test 3 (0 + 0 = 0):          ";
    printList(sol.addTwoNumbers(buildList({0}), buildList({0})));

    // Test 4: 999 + 999 = 1998   → [8, 9, 9, 1]
    cout << "Test 4 (999 + 999 = 1998):   ";
    printList(sol.addTwoNumbers(buildList({9, 9, 9}), buildList({9, 9, 9})));

    // Test 5: Different lengths 342 + 65 = 407  → [7, 0, 4]
    cout << "Test 5 (342 + 65 = 407):     ";
    printList(sol.addTwoNumbers(buildList({2, 4, 3}), buildList({5, 6})));

    // Recursive version tests
    cout << "\n--- Recursive Version ---\n";
    SolutionRecursive solR;
    cout << "Test 1 (342 + 465 = 807):    ";
    printList(solR.addTwoNumbers(buildList({2, 4, 3}), buildList({5, 6, 4})));
    cout << "Test 2 (99 + 1 = 100):       ";
    printList(solR.addTwoNumbers(buildList({9, 9}), buildList({1})));

    return 0;
}

// ============================================================
// COMPLEXITY ANALYSIS — OPTIMIZED (Iterative)
//
// Time:  O(max(M, N))
//   M = length of l1, N = length of l2
//   Each node is visited exactly once.
//   The extra carry-node at the end (if any) is O(1).
//
// Space: O(max(M, N))
//   We build a new result list.
//   Its length is at most max(M, N) + 1 (for carry overflow).
//   We don't count the output space in most analyses → O(1) extra space.
//
// COMPLEXITY ANALYSIS — RECURSIVE
//
// Time:  O(max(M, N))   — same, each position processed once
// Space: O(max(M, N))   — recursion call stack depth equals result length
//
// The iterative approach is preferred in interviews because:
//   1. No risk of stack overflow for very deep lists
//   2. Easier to reason about for interviewers
// ============================================================
