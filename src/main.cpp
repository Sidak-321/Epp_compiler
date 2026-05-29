// src/main.cpp — Phase 3 test

#include <iostream>
#include "unlimitedrational.h"
#include "exprtreenode.h"

int main() {
    std::cout << "=== Phase 3: Expression Tree Node ===" << std::endl;

    // Test 1: 3 + 5
    std::cout << "\nTest 1: 3 + 5" << std::endl;
    UnlimitedRational v3("3", "1");
    UnlimitedRational v5("5", "1");
    ExprTreeNode* left  = new ExprTreeNode(&v3);
    ExprTreeNode* right = new ExprTreeNode(&v5);
    ExprTreeNode* root  = new ExprTreeNode("ADD", left, right);
    root->print();
    delete root;

    // Test 2: 3 + 5 * 2 (MUL deeper = evaluates first)
    std::cout << "\nTest 2: 3 + 5 * 2" << std::endl;
    UnlimitedRational r3("3", "1");
    UnlimitedRational r5("5", "1");
    UnlimitedRational r2("2", "1");
    ExprTreeNode* val3    = new ExprTreeNode(&r3);
    ExprTreeNode* val5    = new ExprTreeNode(&r5);
    ExprTreeNode* val2    = new ExprTreeNode(&r2);
    ExprTreeNode* mulNode = new ExprTreeNode("MUL", val5, val2);
    ExprTreeNode* addNode = new ExprTreeNode("ADD", val3, mulNode);
    addNode->print();
    delete addNode;

    // Test 3: VAR, DEL, RET nodes
    std::cout << "\nTest 3: VAR, DEL, RET nodes" << std::endl;
    ExprTreeNode* varX = new ExprTreeNode("VAR", "x");
    ExprTreeNode* delX = new ExprTreeNode("DEL", "x");
    ExprTreeNode* retX = new ExprTreeNode("RET", "x");
    varX->print();
    delX->print();
    retX->print();
    delete varX;
    delete delX;
    delete retX;

    std::cout << "\n[EPP] Phase 3 complete." << std::endl;
    return 0;
}