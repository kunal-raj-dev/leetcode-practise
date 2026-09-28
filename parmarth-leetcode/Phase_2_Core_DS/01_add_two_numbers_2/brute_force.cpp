// ============================================================
// Problem  : LeetCode #2 — Add Two Numbers
// Approach : Brute Force — Array Extraction & BigInt Simulation
// Time     : O(max(N, M)) — multiple passes over lists and vectors
// Space    : O(N + M)     — stores all digits in intermediate vectors
// ============================================================
// WHY THIS APPROACH EXISTS & THE FATAL OVERFLOW TRAP:
//
// ⚠️ THE FATAL TRAP (DO NOT DO THIS):
// Many beginners attempt to convert the linked lists into standard integers:
//    long long num1 = 0, num2 = 0;
//    long long sum = num1 + num2;
//    // create linked list from sum...
//
// WHY DOES THAT FAIL?
// Constraints state that lists can contain up to 100 nodes!
// In C++:
//   - int maxes out at ~2 * 10^9 (~10 digits)
//   - long long maxes out at ~9 * 10^18 (~19 digits)
// A 100-digit number WILL cause severe integer overflow and crash.
//
// THE WORKING BRUTE FORCE:
// Instead of converting to primitive types, we extract the digits into
// dynamic arrays (std::vector<int>), simulate grade-school column addition
// into another vector, and finally build the new linked list.
// This proves the logic works before optimizing away the extra vectors!
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

ListNode* addTwoNumbersBruteForce(ListNode* l1, ListNode* l2) {
    // --------------------------------------------------------
    // Step 1: Extract all digits from both linked lists into vectors
    // --------------------------------------------------------
    vector<int> digits1;
    vector<int> digits2;

    ListNode* curr1 = l1;
    while (curr1 != nullptr) {
        digits1.push_back(curr1->val);
        curr1 = curr1->next;
    }

    ListNode* curr2 = l2;
    while (curr2 != nullptr) {
        digits2.push_back(curr2->val);
        curr2 = curr2->next;
    }

    // --------------------------------------------------------
    // Step 2: Perform grade-school column addition on the vectors
    // --------------------------------------------------------
    vector<int> resultDigits;
    int i = 0, j = 0;
    int carry = 0;
    int n1 = digits1.size();
    int n2 = digits2.size();

    // Continue as long as there is an unread digit in either list
    // OR a remaining carry from the previous column
    while (i < n1 || j < n2 || carry > 0) {
        int val1 = (i < n1) ? digits1[i++] : 0;
        int val2 = (j < n2) ? digits2[j++] : 0;

        int sum = val1 + val2 + carry;
        resultDigits.push_back(sum % 10); // current column digit (0-9)
        carry = sum / 10;                 // carry forward to next column (0 or 1)
    }

    // --------------------------------------------------------
    // Step 3: Convert the result vector into a new linked list
    // --------------------------------------------------------
    ListNode* dummy = new ListNode(0); // Dummy node simplifies head attachment
    ListNode* curr = dummy;

    for (int d : resultDigits) {
        curr->next = new ListNode(d);
        curr = curr->next;
    }

    ListNode* head = dummy->next;
    delete dummy; // Free sentinel node
    return head;
}

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

    cout << "=== LeetCode #2: Add Two Numbers (Brute Force) ===" << "\n\n";

    // --- Test Case 1: Standard Example (342 + 465 = 807) ---
    // l1: 2 -> 4 -> 3
    // l2: 5 -> 6 -> 4
    ListNode* l1_1 = createList({2, 4, 3});
    ListNode* l2_1 = createList({5, 6, 4});
    cout << "Test 1 Input : l1 = [2, 4, 3], l2 = [5, 6, 4]" << "\n";
    ListNode* ans1 = addTwoNumbersBruteForce(l1_1, l2_1);
    cout << "Test 1 Output: ";
    printList(ans1);
    cout << "Expected     : 7 -> 0 -> 8" << "\n\n";

    // --- Test Case 2: Zeros (0 + 0 = 0) ---
    ListNode* l1_2 = createList({0});
    ListNode* l2_2 = createList({0});
    cout << "Test 2 Input : l1 = [0], l2 = [0]" << "\n";
    ListNode* ans2 = addTwoNumbersBruteForce(l1_2, l2_2);
    cout << "Test 2 Output: ";
    printList(ans2);
    cout << "Expected     : 0" << "\n\n";

    // --- Test Case 3: Unequal Lengths + Multi-digit Carry (9999999 + 9999 = 10009998) ---
    ListNode* l1_3 = createList({9, 9, 9, 9, 9, 9, 9});
    ListNode* l2_3 = createList({9, 9, 9, 9});
    cout << "Test 3 Input : l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]" << "\n";
    ListNode* ans3 = addTwoNumbersBruteForce(l1_3, l2_3);
    cout << "Test 3 Output: ";
    printList(ans3);
    cout << "Expected     : 8 -> 9 -> 9 -> 9 -> 0 -> 0 -> 0 -> 1" << "\n\n";

    // --- Test Case 4: Lingering Final Carry (99 + 1 = 100) ---
    ListNode* l1_4 = createList({9, 9});
    ListNode* l2_4 = createList({1});
    cout << "Test 4 Input : l1 = [9, 9], l2 = [1]" << "\n";
    ListNode* ans4 = addTwoNumbersBruteForce(l1_4, l2_4);
    cout << "Test 4 Output: ";
    printList(ans4);
    cout << "Expected     : 0 -> 0 -> 1" << "\n";

    // Clean up memory
    freeList(l1_1); freeList(l2_1); freeList(ans1);
    freeList(l1_2); freeList(l2_2); freeList(ans2);
    freeList(l1_3); freeList(l2_3); freeList(ans3);
    freeList(l1_4); freeList(l2_4); freeList(ans4);

    return 0;
}

// ============================================================
// TRACE THROUGH TEST 4: l1=[9, 9], l2=[1]
//
// Extraction:
//   digits1 = [9, 9]
//   digits2 = [1]
//
// Iteration 0:
//   val1 = 9, val2 = 1, carry = 0
//   sum = 10 -> digit = 0, new carry = 1
//   resultDigits = [0]
//
// Iteration 1:
//   val1 = 9, val2 = 0, carry = 1
//   sum = 10 -> digit = 0, new carry = 1
//   resultDigits = [0, 0]
//
// Iteration 2 (both lists empty, but carry == 1):
//   val1 = 0, val2 = 0, carry = 1
//   sum = 1 -> digit = 1, new carry = 0
//   resultDigits = [0, 0, 1]
//
// Converted to linked list: 0 -> 0 -> 1 ✅
// ============================================================
