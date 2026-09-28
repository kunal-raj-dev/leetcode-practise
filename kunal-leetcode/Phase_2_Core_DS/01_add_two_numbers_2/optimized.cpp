// ============================================================
// Problem  : LeetCode #2 — Add Two Numbers
// Approach : Optimal — One-Pass Traversal with Sentinel (Dummy Head)
// Time     : O(max(N, M)) — single simultaneous pass over the lists
// Space    : O(1) auxiliary — no intermediate data structures used
//            (O(max(N, M)) space strictly for the output list)
// ============================================================
// THE CORE INSIGHT:
//   1. The digits are stored in REVERSE order. That means the head
//      of each list holds the 1's place (least significant digit).
//      This is a gift! In elementary school column addition, you ALWAYS
//      start adding from the 1's place and move right-to-left.
//      Here, we simply traverse left-to-right (head to tail).
//
//   2. We do NOT need to extract digits into intermediate arrays.
//      We can compute (val1 + val2 + carry) on-the-fly, produce a new
//      node with (sum % 10), update carry = (sum / 10), and advance!
//
//   3. THE SENTINEL (DUMMY HEAD) IDIOM:
//      Allocating a dummy head node eliminates having to write special
//      'if (head == nullptr)' logic for the first node. Every single node
//      (including the very first) is appended via 'curr->next = new Node(...)'.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// Definition for singly-linked list node provided by LeetCode
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        // dummy: acts as an anchor before the real head of the result list
        ListNode* dummy = new ListNode(0);
        ListNode* curr = dummy; // pointer to the last node in our result list

        int carry = 0; // stores carry from the previous column addition (0 or 1)

        // LOOP INVARIANT:
        // Continue as long as:
        //   - l1 still has nodes to process, OR
        //   - l2 still has nodes to process, OR
        //   - there is a non-zero carry remaining (e.g., 99 + 1 = 100)
        while (l1 != nullptr || l2 != nullptr || carry != 0) {

            // If a list has ended (nullptr), its contribution to the sum is 0
            int val1 = (l1 != nullptr) ? l1->val : 0;
            int val2 = (l2 != nullptr) ? l2->val : 0;

            // Total sum at current decimal column
            int sum = val1 + val2 + carry;

            // Calculate new carry for the next column (e.g., 15 / 10 = 1)
            carry = sum / 10;

            // Create new node with the current column's single digit (e.g., 15 % 10 = 5)
            curr->next = new ListNode(sum % 10);
            curr = curr->next; // advance the tail pointer

            // Advance input list pointers if they are not already at the end
            if (l1 != nullptr) l1 = l1->next;
            if (l2 != nullptr) l2 = l2->next;
        }

        // The real result starts right after the dummy node
        ListNode* result = dummy->next;

        // Delete the temporary sentinel node to prevent memory leaks
        delete dummy;

        return result;
    }
};

// ============================================================
// HELPER FUNCTIONS FOR LOCAL TESTING
// ============================================================

// Creates a linked list from an array of integers
ListNode* createList(const vector<int>& values) {
    ListNode* dummy = new ListNode(0);
    ListNode* curr = dummy;
    for (int v : values) {
        curr->next = new ListNode(v);
        curr = curr->next;
    }
    ListNode* head = dummy->next;
    delete dummy;
    return head;
}

// Prints the linked list in format: 7 -> 0 -> 8
void printList(ListNode* head) {
    if (!head) {
        cout << "[]" << endl;
        return;
    }
    ListNode* curr = head;
    while (curr != nullptr) {
        cout << curr->val;
        if (curr->next != nullptr) cout << " -> ";
        curr = curr->next;
    }
    cout << endl;
}

// Frees all allocated memory in the linked list
void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// ============================================================
// MAIN FUNCTION — COMPREHENSIVE TEST SUITE
// ============================================================

int main() {
    // Fast I/O lines
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    Solution solver;

    cout << "=== LeetCode #2: Add Two Numbers (Optimized One-Pass) ===" << "\n\n";

    // --- Test Case 1: Standard Example (342 + 465 = 807) ---
    // l1: 2 -> 4 -> 3
    // l2: 5 -> 6 -> 4
    ListNode* l1_1 = createList({2, 4, 3});
    ListNode* l2_1 = createList({5, 6, 4});
    cout << "Test 1 Input : l1 = [2, 4, 3], l2 = [5, 6, 4]" << "\n";
    ListNode* ans1 = solver.addTwoNumbers(l1_1, l2_1);
    cout << "Test 1 Output: ";
    printList(ans1);
    cout << "Expected     : 7 -> 0 -> 8" << "\n\n";

    // --- Test Case 2: Both Zeros (0 + 0 = 0) ---
    ListNode* l1_2 = createList({0});
    ListNode* l2_2 = createList({0});
    cout << "Test 2 Input : l1 = [0], l2 = [0]" << "\n";
    ListNode* ans2 = solver.addTwoNumbers(l1_2, l2_2);
    cout << "Test 2 Output: ";
    printList(ans2);
    cout << "Expected     : 0" << "\n\n";

    // --- Test Case 3: Unequal Lengths + Multi-digit Carry (9999999 + 9999 = 10009998) ---
    ListNode* l1_3 = createList({9, 9, 9, 9, 9, 9, 9});
    ListNode* l2_3 = createList({9, 9, 9, 9});
    cout << "Test 3 Input : l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]" << "\n";
    ListNode* ans3 = solver.addTwoNumbers(l1_3, l2_3);
    cout << "Test 3 Output: ";
    printList(ans3);
    cout << "Expected     : 8 -> 9 -> 9 -> 9 -> 0 -> 0 -> 0 -> 1" << "\n\n";

    // --- Test Case 4: Lingering Final Carry (99 + 1 = 100) ---
    ListNode* l1_4 = createList({9, 9});
    ListNode* l2_4 = createList({1});
    cout << "Test 4 Input : l1 = [9, 9], l2 = [1]" << "\n";
    ListNode* ans4 = solver.addTwoNumbers(l1_4, l2_4);
    cout << "Test 4 Output: ";
    printList(ans4);
    cout << "Expected     : 0 -> 0 -> 1" << "\n\n";

    // --- Test Case 5: Single Digit Carry (5 + 5 = 10) ---
    ListNode* l1_5 = createList({5});
    ListNode* l2_5 = createList({5});
    cout << "Test 5 Input : l1 = [5], l2 = [5]" << "\n";
    ListNode* ans5 = solver.addTwoNumbers(l1_5, l2_5);
    cout << "Test 5 Output: ";
    printList(ans5);
    cout << "Expected     : 0 -> 1" << "\n";

    // Clean up memory
    freeList(l1_1); freeList(l2_1); freeList(ans1);
    freeList(l1_2); freeList(l2_2); freeList(ans2);
    freeList(l1_3); freeList(l2_3); freeList(ans3);
    freeList(l1_4); freeList(l2_4); freeList(ans4);
    freeList(l1_5); freeList(l2_5); freeList(ans5);

    return 0;
}

// ============================================================
// STEP-BY-STEP TRACE (Mental Walkthrough of Test 1):
//
// l1 = [2, 4, 3],  l2 = [5, 6, 4]
//
// dummy = [0]
// curr = dummy
// carry = 0
//
// Iteration 1:
//   l1 points to 2, l2 points to 5, carry = 0
//   sum = 2 + 5 + 0 = 7
//   carry = 7 / 10 = 0
//   curr->next = new Node(7)
//   curr moves to Node(7)
//   l1 moves to 4, l2 moves to 6
//
// Iteration 2:
//   l1 points to 4, l2 points to 6, carry = 0
//   sum = 4 + 6 + 0 = 10
//   carry = 10 / 10 = 1
//   curr->next = new Node(0)
//   curr moves to Node(0)
//   l1 moves to 3, l2 moves to 4
//
// Iteration 3:
//   l1 points to 3, l2 points to 4, carry = 1
//   sum = 3 + 4 + 1 = 8
//   carry = 8 / 10 = 0
//   curr->next = new Node(8)
//   curr moves to Node(8)
//   l1 moves to nullptr, l2 moves to nullptr
//
// Iteration 4:
//   l1 == nullptr && l2 == nullptr && carry == 0
//   Loop terminates!
//
// result = dummy->next = Node(7)
// Output: 7 -> 0 -> 8  ✅
// ============================================================
