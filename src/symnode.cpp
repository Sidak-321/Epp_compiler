// src/symnode.cpp

#include "symnode.h"

SymNode::SymNode(std::string k, UnlimitedRational* v, int slot) {
    key     = k;
    value   = new UnlimitedRational(*v);  // make our own copy
    mem_loc = slot;
    left    = nullptr;
    right   = nullptr;
}

SymNode::~SymNode() {
    delete value;
    // Note: we do NOT delete left/right here
    // The BST (SymTable) manages the tree structure and deletions
    // If we deleted children here, removing one node would delete the whole subtree
}