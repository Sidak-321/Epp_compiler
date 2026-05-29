// include/symtable.h
// BST-based symbol table — maps variable names to memory slots and values

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "symnode.h"
#include <string>

class SymTable {
private:
    SymNode* root;   // root of the BST
    int      count;  // number of variables currently stored

    // Private helpers — internal recursive functions
    // We separate public interface from recursive implementation
    SymNode* insert_rec(SymNode* node, std::string key,
                        UnlimitedRational* val, int slot);

    SymNode* remove_rec(SymNode* node, std::string key);

    SymNode* search_rec(SymNode* node, std::string key) const;

    // Find the smallest node in a subtree (used in BST delete)
    SymNode* find_min(SymNode* node) const;

    // Recursively delete all nodes (used in destructor)
    void destroy(SymNode* node);

public:
    SymTable();
    ~SymTable();

    // Insert a new variable
    void insert(std::string key, UnlimitedRational* val, int slot);

    // Find a variable — returns its SymNode or nullptr if not found
    SymNode* search(std::string key) const;

    // Remove a variable from the BST
    // Returns the memory slot that was freed (so MinHeap can reclaim it)
    int remove(std::string key);

    // How many variables are currently stored
    int get_size() const;

    // Print all variables in alphabetical order (in-order traversal)
    void print_all(SymNode* node) const;
};

#endif