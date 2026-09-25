// ============================================================
// Problem  : Add Two Numbers (LeetCode #2)
// Approach : Recursive — Each call handles one digit position
// Time     : O(max(M,N)) — one recursive call per digit
// Space    : O(max(M,N)) — recursion stack + result list
// ============================================================
// THE CORE INSIGHT:
//   Addition is inherently recursive:
//     "Add these two digits + carry, then solve the rest."
//   Each recursive call handles one digit position and passes
//   the carry forward to the next call.
//
// NOTE: Both the iterative and recursive approaches have the
// same time/space complexity. This file shows the recursive
// version for elegance and to practice recursion on linked
// lists. The iterative version (brute_force.cpp) is preferred
// in interviews for its simplicity and no stack overflow risk.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// RECURSIVE APPROACH:
// Think of it as: "Handle current digit, let recursion handle the rest"
//
// Base case: both lists empty AND carry is 0 → nothing left to do → return NULL
// Recursive case: compute this digit's sum, create node, recurse for remaining digits
//
// carry has a DEFAULT VALUE of 0 — so the initial call doesn't need to pass it.
// Default parameters: if caller doesn't provide 'carry', it defaults to 0.
// Written as: int carry = 0  in the parameter list.

ListNode* addTwoNumbers(ListNode* l1, ListNode* l2, int carry = 0) {

    // BASE CASE: nothing left to process
    // Both lists exhausted AND no carry remaining → we're done
    if (!l1 && !l2 && !carry) return nullptr;

    // RECURSIVE CASE:
    // Step 1: Compute sum for current digit position
    int sum = carry;
    if (l1) sum += l1->val;   // add l1's digit (if it exists)
    if (l2) sum += l2->val;   // add l2's digit (if it exists)

    // Step 2: Create the node for this digit
    // sum % 10 = the digit to write (e.g., 15 → 5)
    ListNode* node = new ListNode(sum % 10);

    // Step 3: Recurse for the NEXT digit position
    // - Advance l1 to l1->next (if l1 exists, else stay nullptr)
    // - Advance l2 to l2->next (if l2 exists, else stay nullptr)
    // - Pass the carry (sum / 10) to the next position
    //
    // The ternary operator: (condition) ? value_if_true : value_if_false
    //   l1 ? l1->next : nullptr
    //   = "if l1 is not null, go to l1->next; otherwise stay null"
    node->next = addTwoNumbers(
        l1 ? l1->next : nullptr,    // advance l1 (or stay null)
        l2 ? l2->next : nullptr,    // advance l2 (or stay null)
        sum / 10                     // new carry
    );

    // Step 4: Return this node — it links to the recursively-built rest
    return node;
}

// ============================================================
// Helper: Build a linked list from a vector (for testing)
// ============================================================
ListNode* buildList(vector<int> digits) {
    ListNode* dummy = new ListNode(0);
    ListNode* curr = dummy;
    for (int d : digits) {
        curr->next = new ListNode(d);
        curr = curr->next;
    }
    return dummy->next;
}

// ============================================================
// Helper: Print a linked list (for testing)
// ============================================================
void printList(ListNode* head) {
    cout << "[";
    while (head) {
        cout << head->val;
        if (head->next) cout << ", ";
        head = head->next;
    }
    cout << "]" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // --- Test Case 1: Basic addition ---
    // 342 + 465 = 807
    ListNode* l1 = buildList({2, 4, 3});
    ListNode* l2 = buildList({5, 6, 4});
    cout << "Test 1: ";
    printList(addTwoNumbers(l1, l2));
    // Expected: [7, 0, 8]

    // --- Test Case 2: Both are zero ---
    // 0 + 0 = 0
    ListNode* l3 = buildList({0});
    ListNode* l4 = buildList({0});
    cout << "Test 2: ";
    printList(addTwoNumbers(l3, l4));
    // Expected: [0]

    // --- Test Case 3: Carry creates extra digit ---
    // 999 + 1 = 1000
    ListNode* l5 = buildList({9, 9, 9});
    ListNode* l6 = buildList({1});
    cout << "Test 3: ";
    printList(addTwoNumbers(l5, l6));
    // Expected: [0, 0, 0, 1]

    // --- Test Case 4: Different lengths with carry chain ---
    // 9999999 + 9999 = 10009998
    ListNode* l7 = buildList({9, 9, 9, 9, 9, 9, 9});
    ListNode* l8 = buildList({9, 9, 9, 9});
    cout << "Test 4: ";
    printList(addTwoNumbers(l7, l8));
    // Expected: [8, 9, 9, 9, 0, 0, 0, 1]

    // --- Test Case 5: Single digits with carry ---
    // 5 + 5 = 10
    ListNode* l9 = buildList({5});
    ListNode* l10 = buildList({5});
    cout << "Test 5: ";
    printList(addTwoNumbers(l9, l10));
    // Expected: [0, 1]

    return 0;
}

// ============================================================
// RECURSION TRACE — TEST 1:
//
// addTwoNumbers([2]→[4]→[3], [5]→[6]→[4], carry=0)
//   sum = 0+2+5 = 7 → node(7)
//   node->next = addTwoNumbers([4]→[3], [6]→[4], carry=0)
//     sum = 0+4+6 = 10 → node(0)
//     node->next = addTwoNumbers([3], [4], carry=1)
//       sum = 1+3+4 = 8 → node(8)
//       node->next = addTwoNumbers(NULL, NULL, carry=0)
//         → returns nullptr (base case: !l1 && !l2 && !carry)
//       return node(8) → [8] → NULL
//     return node(0) → [0] → [8] → NULL
//   return node(7) → [7] → [0] → [8] → NULL  ✅
//
// ─────────────────────────────────────────────────────────
// RECURSION TRACE — TEST 3 (999 + 1):
//
// addTwoNumbers([9]→[9]→[9], [1], carry=0)
//   sum = 0+9+1 = 10 → node(0)
//   node->next = addTwoNumbers([9]→[9], NULL, carry=1)
//     sum = 1+9+0 = 10 → node(0)
//     node->next = addTwoNumbers([9], NULL, carry=1)
//       sum = 1+9+0 = 10 → node(0)
//       node->next = addTwoNumbers(NULL, NULL, carry=1)
//         sum = 1+0+0 = 1 → node(1)
//         node->next = addTwoNumbers(NULL, NULL, carry=0)
//           → returns nullptr (base case)
//         return node(1) → [1] → NULL
//       return node(0) → [0] → [1] → NULL
//     return node(0) → [0] → [0] → [1] → NULL
//   return node(0) → [0] → [0] → [0] → [1] → NULL  ✅
// ============================================================
