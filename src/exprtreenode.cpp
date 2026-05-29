// src/exprtreenode.cpp

#include "exprtreenode.h"
#include <iostream>
#include <string>

// ─── CONSTRUCTOR: Operator node (ADD, SUB, MUL, DIV) ─────────────────────────
// Example: building the "+" in "3 + 5"
//   ExprTreeNode* node = new ExprTreeNode("ADD", leftChild, rightChild);

ExprTreeNode::ExprTreeNode(std::string node_type,
                           ExprTreeNode* left_child,
                           ExprTreeNode* right_child) {
    type  = node_type;
    left  = left_child;
    right = right_child;
    value = nullptr;   // operators don't hold a numeric value
    var_id = "";
}

// ─── CONSTRUCTOR: VAL node (a number) ────────────────────────────────────────
// Example: the "3" in "3 + 5"
//   UnlimitedRational* r = new UnlimitedRational("3", "1");
//   ExprTreeNode* node = new ExprTreeNode(r);

ExprTreeNode::ExprTreeNode(UnlimitedRational* val) {
    type  = "VAL";
    value = new UnlimitedRational(*val);  // make our own copy
    left  = nullptr;
    right = nullptr;
    var_id = "";
}

// ─── CONSTRUCTOR: VAR / DEL / RET node (a variable name) ─────────────────────
// Example: "VAR" for x in "x = 3 + 5"
//          "DEL" for "del x"
//          "RET" for "ret x"
//   ExprTreeNode* node = new ExprTreeNode("VAR", "x");

ExprTreeNode::ExprTreeNode(std::string node_type, std::string id) {
    type   = node_type;
    var_id = id;
    value  = nullptr;
    left   = nullptr;
    right  = nullptr;
}

// ─── DESTRUCTOR ───────────────────────────────────────────────────────────────
// Deleting a node automatically deletes its entire subtree
// This is called a "recursive destructor" — very common in tree structures

ExprTreeNode::~ExprTreeNode() {
    delete value;   // safe to delete nullptr
    delete left;    // recursively deletes left subtree
    delete right;   // recursively deletes right subtree
}

// ─── PRINT ────────────────────────────────────────────────────────────────────
// Prints the tree with indentation to show structure
// Called as: root->print(0)
// Each level adds 4 spaces of indent

void ExprTreeNode::print(int indent) const {
    std::string pad(indent, ' ');  // indent spaces

    if (type == "VAL") {
        std::cout << pad << "VAL(" << value->to_string() << ")" << std::endl;
    }
    else if (type == "VAR") {
        std::cout << pad << "VAR(" << var_id << ")" << std::endl;
    }
    else if (type == "DEL") {
        std::cout << pad << "DEL(" << var_id << ")" << std::endl;
    }
    else if (type == "RET") {
        std::cout << pad << "RET(" << var_id << ")" << std::endl;
    }
    else {
        // Operator node — print self, then children indented
        std::cout << pad << type << std::endl;
        if (left)  left->print(indent + 4);
        if (right) right->print(indent + 4);
    }
}