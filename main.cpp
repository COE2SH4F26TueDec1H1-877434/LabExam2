#include <iostream>
#include "ScoreBoard.hpp"
#include "WeightedBoard.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    int initV = static_cast<int>(10);
    int extV = static_cast<int>(3);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        ScoreBoard pDef;
        if (approxEqual(static_cast<double>(pDef.totalPoints()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getScore(0)), 0.0)) {
            cout << "[PASS] TC 01: ScoreBoard default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: ScoreBoard default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        ScoreBoard p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getScore(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getScore(2)), static_cast<double>(initV + static_cast<int>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getScore(1) == static_cast<int>(initV + static_cast<int>(1))) {
            // Compare against a second identical instance
            ScoreBoard pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.totalPoints()), static_cast<double>(pCheck.totalPoints())) &&
                !approxEqual(static_cast<double>(p1.totalPoints()), 0.0)) {
                cout << "[PASS] TC 02: ScoreBoard parameterized constructor & totalPoints()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: ScoreBoard totalPoints() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: ScoreBoard element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        ScoreBoard orig(3, initV, extV);
        ScoreBoard copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.totalPoints()), static_cast<double>(orig.totalPoints())) &&
            approxEqual(static_cast<double>(copyObj.getScore(1)), static_cast<double>(orig.getScore(1))) &&
            !approxEqual(static_cast<double>(copyObj.totalPoints()), 0.0)) {
            cout << "[PASS] TC 03: ScoreBoard copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: ScoreBoard copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        ScoreBoard a(3, initV, extV);
        ScoreBoard b(2, initV, extV);
        b = a;
        ScoreBoard* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.totalPoints()), static_cast<double>(a.totalPoints())) &&
            approxEqual(static_cast<double>(b.getScore(2)), static_cast<double>(a.getScore(2))) &&
            !approxEqual(static_cast<double>(b.totalPoints()), 0.0)) {
            cout << "[PASS] TC 04: ScoreBoard copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: ScoreBoard copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            ScoreBoard temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: ScoreBoard destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: ScoreBoard destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        ScoreBoard baseObj(3, initV, extV);
        WeightedBoard childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.totalPoints()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.totalPoints()), static_cast<double>(baseObj.totalPoints()))) {
            cout << "[PASS] TC 06: WeightedBoard overridden totalPoints()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: WeightedBoard overridden totalPoints() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        WeightedBoard directChild(3, initV, extV);
        ScoreBoard* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->totalPoints()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->totalPoints()), static_cast<double>(directChild.totalPoints()))) {
            cout << "[PASS] TC 07: WeightedBoard polymorphic dispatch via ScoreBoard*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: WeightedBoard polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        WeightedBoard c1(3, initV, extV);
        WeightedBoard c2(c1);
        if (approxEqual(static_cast<double>(c1.totalPoints()), static_cast<double>(c2.totalPoints())) &&
            !approxEqual(static_cast<double>(c2.totalPoints()), 0.0)) {
            cout << "[PASS] TC 08: WeightedBoard copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: WeightedBoard copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        WeightedBoard c1(3, initV, extV);
        WeightedBoard c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.totalPoints()), static_cast<double>(c2.totalPoints())) &&
            !approxEqual(static_cast<double>(c2.totalPoints()), 0.0)) {
            cout << "[PASS] TC 09: WeightedBoard copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: WeightedBoard copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        ScoreBoard* heapChild = new WeightedBoard(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: WeightedBoard virtual destructor via ScoreBoard*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: WeightedBoard virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
