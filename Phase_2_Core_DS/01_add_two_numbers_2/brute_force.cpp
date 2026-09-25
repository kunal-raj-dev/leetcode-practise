// ============================================================
// Problem  : Add Two Numbers (LeetCode #2)
// Approach : Iterative — Digit-by-Digit Addition with Carry
// Time     : O(max(M,N)) — single pass through both lists
// Space    : O(max(M,N)) — new list to hold the result
// ============================================================
// This IS the brute force AND it's already optimal.
// You cannot avoid visiting every digit — so O(max(M,N)) is
// the best possible time complexity for this problem.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// Definition of a singly-linked list node.
// Each node holds ONE digit (0-9) and a pointer to the next node.
//
// FIRST TIME SEEING A LINKED LIST? Here's the mental model:
//   [2|•] → [4|•] → [3|NULL]
//   Each box = one ListNode
//   "val" = the number inside
//   "next" = the arrow pointing to the next box (or NULL = end)
struct ListNode {
    int val;            // the digit stored in this node (0-9)
    ListNode* next;     // pointer to the next node (nullptr = end of list)

    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    // DUMMY HEAD TRICK:
    // When building a new linked list, the first node is awkward
    // because there's no "previous" node to attach it to.
    // Solution: create a fake node (dummy) at position -1,
    // build after it, then return dummy->next to skip it.
    ListNode* dummy = new ListNode(0);

    // "curr" always points to the LAST node we've built so far.
    // We'll attach new nodes after curr, then advance curr.
    ListNode* curr = dummy;

    // carry starts at 0 — just like column addition by hand
    int carry = 0;

    // WHY three conditions?
    //   l1        → first list still has digits to process
    //   l2        → second list still has digits to process
    //   carry     → e.g., 999 + 1 = 1000. After both lists end,
    //               carry=1 needs to become an extra node [1]
    // If you forget "|| carry", you'll fail on cases like 999+1.
    while (l1 || l2 || carry) {

        // Start with carry from previous digit
        int sum = carry;

        // Add l1's digit if l1 hasn't been exhausted
        if (l1) {
            sum += l1->val;   // add the digit
            l1 = l1->next;    // advance to next digit
        }
        // If l1 is nullptr (exhausted), we just don't add anything.
        // This is equivalent to treating missing digits as 0.

        // Add l2's digit if l2 hasn't been exhausted
        if (l2) {
            sum += l2->val;
            l2 = l2->next;
        }

        // ELEMENTARY SCHOOL MATH:
        //   sum = 15  →  digit = 15 % 10 = 5  (write down 5)
        //                carry = 15 / 10 = 1  (carry the 1)
        //   sum = 7   →  digit = 7 % 10 = 7   (write down 7)
        //                carry = 7 / 10 = 0   (no carry)
        curr->next = new ListNode(sum % 10);  // create node with this digit
        curr = curr->next;                     // advance our build pointer
        carry = sum / 10;                      // update carry for next round
    }

    // Skip the dummy node — return the actual first result node
    return dummy->next;
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

    // --- Test Case 3: Carry propagates to new digit ---
    // 999 + 1 = 1000
    ListNode* l5 = buildList({9, 9, 9});
    ListNode* l6 = buildList({1});
    cout << "Test 3: ";
    printList(addTwoNumbers(l5, l6));
    // Expected: [0, 0, 0, 1]

    // --- Test Case 4: Different lengths ---
    // 99 + 999 = 1098
    ListNode* l7 = buildList({9, 9});
    ListNode* l8 = buildList({9, 9, 9});
    cout << "Test 4: ";
    printList(addTwoNumbers(l7, l8));
    // Expected: [8, 9, 0, 1]

    return 0;
}

// ============================================================
// TRACE THROUGH TEST 1 (learn to mentally trace code):
//
// l1 = [2] → [4] → [3]   (342)
// l2 = [5] → [6] → [4]   (465)
// carry = 0
//
// Iteration 1:
//   sum = 0 + 2 + 5 = 7
//   digit = 7 % 10 = 7,  carry = 7 / 10 = 0
//   result so far: dummy → [7]
//   l1 → [4], l2 → [6]
//
// Iteration 2:
//   sum = 0 + 4 + 6 = 10
//   digit = 10 % 10 = 0, carry = 10 / 10 = 1
//   result so far: dummy → [7] → [0]
//   l1 → [3], l2 → [4]
//
// Iteration 3:
//   sum = 1 + 3 + 4 = 8
//   digit = 8 % 10 = 8,  carry = 8 / 10 = 0
//   result so far: dummy → [7] → [0] → [8]
//   l1 → NULL, l2 → NULL
//
// Loop ends (l1=NULL, l2=NULL, carry=0).
// Return dummy->next = [7] → [0] → [8]  ✅
// ============================================================

// ============================================================
// TRACE THROUGH TEST 3 (the tricky carry case):
//
// l1 = [9] → [9] → [9]   (999)
// l2 = [1]                (1)
// carry = 0
//
// Iteration 1:
//   sum = 0 + 9 + 1 = 10
//   digit = 0, carry = 1
//   result: dummy → [0]
//   l1 → [9], l2 → NULL
//
// Iteration 2:
//   sum = 1 + 9 + 0 = 10     (l2 is NULL, treated as 0)
//   digit = 0, carry = 1
//   result: dummy → [0] → [0]
//   l1 → [9], l2 stays NULL
//
// Iteration 3:
//   sum = 1 + 9 + 0 = 10
//   digit = 0, carry = 1
//   result: dummy → [0] → [0] → [0]
//   l1 → NULL
//
// Iteration 4:
//   sum = 1 + 0 + 0 = 1      (both NULL, but carry=1 keeps loop alive!)
//   digit = 1, carry = 0
//   result: dummy → [0] → [0] → [0] → [1]
//
// Loop ends. Return [0, 0, 0, 1]  ✅  (= 1000)
// ============================================================
