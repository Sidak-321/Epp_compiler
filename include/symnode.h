// include/symnode.h
// A single node in the Symbol Table BST

#ifndef SYMNODE_H
#define SYMNODE_H

#include "unlimitedrational.h"
#include <string>

class SymNode {
public:
    std::string        key;      // variable name — used for BST ordering
    UnlimitedRational* value;    // the variable's current value
    int                mem_loc;  // memory slot assigned to this variable

    SymNode* left;   // BST left child  (key < this->key)
    SymNode* right;  // BST right child (key > this->key)

    // Constructor
    SymNode(std::string k, UnlimitedRational* v, int slot);

    // Destructor
    ~SymNode();
};

#endif