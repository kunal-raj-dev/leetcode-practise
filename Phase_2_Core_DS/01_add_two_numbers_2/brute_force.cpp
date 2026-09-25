// ============================================================
// Problem : Add Two Numbers (LeetCode #2)
// Link    : https://leetcode.com/problems/add-two-numbers
// Approach: BRUTE FORCE — Naive simulation WITHOUT dummy head
//           (To show why the dummy head trick exists)
// Time    : O(max(M, N))
// Space   : O(max(M, N))
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// Definition for singly-linked list node.
// This is given to you in LeetCode — don't write it in the actual submission.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {

        // --- Step 1: Initialize tracking variables ---
        int carry = 0;         // carry from the previous digit's sum (starts at 0)
        ListNode* head = nullptr;  // head of the result list (we don't know it yet)
        ListNode* tail = nullptr;  // tail pointer — last node we added to result

        // --- Step 2: Loop until BOTH lists are done AND no carry left ---
        // Why all three conditions?
        //   l1 != nullptr    → l1 still has digits to process
        //   l2 != nullptr    → l2 still has digits to process
        //   carry != 0       → there's a leftover carry to handle (e.g. 99 + 1 = 100)
        while (l1 != nullptr || l2 != nullptr || carry != 0) {

            // Get the digit from l1 (use 0 if l1 is exhausted)
            int d1 = (l1 != nullptr) ? l1->val : 0;

            // Get the digit from l2 (use 0 if l2 is exhausted)
            int d2 = (l2 != nullptr) ? l2->val : 0;

            // Add both digits + any carry from previous round
            int sum = d1 + d2 + carry;

            // carry for next round: if sum >= 10, carry = 1, else carry = 0
            // Example: sum = 17 → carry = 1, digit = 7
            // Example: sum = 7  → carry = 0, digit = 7
            carry = sum / 10;

            // The actual digit to store in this result node
            int digit = sum % 10;

            // --- Step 3: Create a new node and attach it to the result list ---
            // BRUTE FORCE: We manually handle the first node (head) specially
            // This is the "pain point" that the dummy head trick eliminates.
            ListNode* newNode = new ListNode(digit);

            if (head == nullptr) {
                // First node — initialize both head and tail
                head = newNode;
                tail = newNode;
            } else {
                // Subsequent nodes — link to the tail and update tail
                tail->next = newNode;
                tail = tail->next;
            }

            // --- Step 4: Advance both list pointers (if not already null) ---
            if (l1 != nullptr) l1 = l1->next;
            if (l2 != nullptr) l2 = l2->next;
        }

        // Return the head of the constructed result list
        return head;
    }
};

// ============================================================
// HOW TO TEST — Manual linked list construction helper
// ============================================================

// Helper to build a linked list from a vector (for testing)
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

// Helper to print a linked list
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

    // Test 1: 342 + 465 = 807
    // l1 = [2, 4, 3], l2 = [5, 6, 4]
    // Expected: [7, 0, 8]
    ListNode* l1 = buildList({2, 4, 3});
    ListNode* l2 = buildList({5, 6, 4});
    cout << "Test 1 — 342 + 465 = 807\nResult: ";
    printList(sol.addTwoNumbers(l1, l2));

    // Test 2: 99 + 1 = 100 (carry at end!)
    // l1 = [9, 9], l2 = [1]
    // Expected: [0, 0, 1]
    ListNode* l3 = buildList({9, 9});
    ListNode* l4 = buildList({1});
    cout << "Test 2 — 99 + 1 = 100\nResult: ";
    printList(sol.addTwoNumbers(l3, l4));

    // Test 3: 0 + 0 = 0
    ListNode* l5 = buildList({0});
    ListNode* l6 = buildList({0});
    cout << "Test 3 — 0 + 0 = 0\nResult: ";
    printList(sol.addTwoNumbers(l5, l6));

    // Test 4: 999 + 999 = 1998
    // Expected: [8, 9, 9, 1]
    ListNode* l7 = buildList({9, 9, 9});
    ListNode* l8 = buildList({9, 9, 9});
    cout << "Test 4 — 999 + 999 = 1998\nResult: ";
    printList(sol.addTwoNumbers(l7, l8));

    return 0;
}

// ============================================================
// COMPLEXITY ANALYSIS
// Time:  O(max(M, N)) — we visit every node in both lists once
// Space: O(max(M, N)) — result list has at most max(M,N)+1 nodes
//
// PROBLEM WITH THIS APPROACH:
//   The `if (head == nullptr)` check inside the loop is messy.
//   Every iteration has an extra branch just to handle the first node.
//   The OPTIMIZED solution eliminates this using a "dummy head" node.
// ============================================================
