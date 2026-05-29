// src/main.cpp — Phase 5 test

#include <iostream>
#include "minheap.h"
#include "parser.h"
#include "symtable.h"

int main() {

    // ── Test 1: MinHeap ───────────────────────────────────────────────────────
    std::cout << "=== Phase 5: MinHeap ===" << std::endl;

    MinHeap heap(5);  // 5 memory slots: 0,1,2,3,4
    std::cout << "Initial: "; heap.print();

    int s1 = heap.pop();
    std::cout << "pop() = " << s1 << " | "; heap.print();  // 0

    int s2 = heap.pop();
    std::cout << "pop() = " << s2 << " | "; heap.print();  // 1

    heap.push(s1);  // return slot 0
    std::cout << "push(0)  | "; heap.print();  // 0 back

    int s3 = heap.pop();
    std::cout << "pop() = " << s3 << " | "; heap.print();  // 0 reused

    // ── Test 2: Parser ────────────────────────────────────────────────────────
    std::cout << "\n=== Phase 5: Parser ===" << std::endl;

    SymTable symtable;
    MinHeap  heap2(10);
    Parser   parser(&symtable, &heap2);

    // Test: x = 3 + 5
    std::cout << "\nParsing: x = 3 + 5" << std::endl;
    ExprTreeNode* t1 = parser.parse_line("x = 3 + 5");
    if (t1) { t1->print(); delete t1; }

    // Test: y = 10
    std::cout << "\nParsing: y = 10" << std::endl;
    ExprTreeNode* t2 = parser.parse_line("y = 10");
    if (t2) { t2->print(); delete t2; }

    // Test: del x
    std::cout << "\nParsing: del x" << std::endl;
    ExprTreeNode* t3 = parser.parse_line("del x");
    if (t3) { t3->print(); delete t3; }

    // Test: ret y
    std::cout << "\nParsing: ret y" << std::endl;
    ExprTreeNode* t4 = parser.parse_line("ret y");
    if (t4) { t4->print(); delete t4; }

    // Verify heap recycled slot 0 after del x
    std::cout << "\nHeap after del x: "; heap2.print();
    std::cout << "SymTable size: " << symtable.get_size() << std::endl;

    std::cout << "\n[EPP] Phase 5 complete." << std::endl;
    return 0;
}