// src/symtable.cpp

#include "symtable.h"
#include <iostream>

// ─── CONSTRUCTOR / DESTRUCTOR ─────────────────────────────────────────────────

SymTable::SymTable() {
    root  = nullptr;
    count = 0;
}

SymTable::~SymTable() {
    destroy(root);
}

// Recursively delete every node in the tree
void SymTable::destroy(SymNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// ─── GET SIZE ─────────────────────────────────────────────────────────────────

int SymTable::get_size() const {
    return count;
}

// ─── INSERT ───────────────────────────────────────────────────────────────────
// Public insert — just calls the recursive helper starting at root

void SymTable::insert(std::string key, UnlimitedRational* val, int slot) {
    root = insert_rec(root, key, val, slot);
    count++;
}

// Recursive insert:
// Walk the tree — go left if key is smaller, right if larger
// When we hit nullptr, that's where the new node goes

SymNode* SymTable::insert_rec(SymNode* node, std::string key,
                               UnlimitedRational* val, int slot) {
    if (node == nullptr) {
        // Found the empty spot — create node here
        return new SymNode(key, val, slot);
    }

    if (key < node->key) {
        // key is alphabetically smaller — go left
        node->left = insert_rec(node->left, key, val, slot);
    }
    else if (key > node->key) {
        // key is alphabetically larger — go right
        node->right = insert_rec(node->right, key, val, slot);
    }
    // if key == node->key: variable already exists, do nothing

    return node;  // return the (unchanged) current node
}

// ─── SEARCH ───────────────────────────────────────────────────────────────────

SymNode* SymTable::search(std::string key) const {
    return search_rec(root, key);
}

SymNode* SymTable::search_rec(SymNode* node, std::string key) const {
    if (node == nullptr) return nullptr;  // not found

    if (key == node->key) return node;    // found!

    if (key < node->key)
        return search_rec(node->left, key);   // go left
    else
        return search_rec(node->right, key);  // go right
}

// ─── FIND MIN ─────────────────────────────────────────────────────────────────
// Returns the leftmost node in a subtree
// Used during BST delete to find the in-order successor

SymNode* SymTable::find_min(SymNode* node) const {
    while (node->left != nullptr) node = node->left;
    return node;
}

// ─── REMOVE ───────────────────────────────────────────────────────────────────
// Public remove — returns the freed memory slot

int SymTable::remove(std::string key) {
    // First find the node to get its mem_loc before deleting
    SymNode* target = search(key);
    if (target == nullptr) return -1;  // variable not found

    int freed_slot = target->mem_loc;
    root = remove_rec(root, key);
    count--;
    return freed_slot;
}

// BST delete has 3 cases:
// Case 1: Node has no children → just delete it
// Case 2: Node has one child  → replace node with that child
// Case 3: Node has two children → replace with in-order successor
//         (smallest node in right subtree), then delete the successor

SymNode* SymTable::remove_rec(SymNode* node, std::string key) {
    if (node == nullptr) return nullptr;

    if (key < node->key) {
        // Target is in left subtree
        node->left = remove_rec(node->left, key);
    }
    else if (key > node->key) {
        // Target is in right subtree
        node->right = remove_rec(node->right, key);
    }
    else {
        // Found the node to delete
        
        // Case 1 & 2: No left child — replace with right child (or nullptr)
        if (node->left == nullptr) {
            SymNode* temp = node->right;
            delete node;
            return temp;
        }
        // Case 2: No right child — replace with left child
        else if (node->right == nullptr) {
            SymNode* temp = node->left;
            delete node;
            return temp;
        }
        // Case 3: Two children
        // Find in-order successor (smallest in right subtree)
        // Copy its data into current node, then delete the successor
        else {
            SymNode* successor = find_min(node->right);
            
            // Copy successor's data into this node
            node->key     = successor->key;
            delete node->value;
            node->value   = new UnlimitedRational(*successor->value);
            node->mem_loc = successor->mem_loc;

            // Now delete the successor from right subtree
            node->right = remove_rec(node->right, successor->key);
        }
    }
    return node;
}

// ─── PRINT ALL (in-order traversal) ──────────────────────────────────────────
// In-order = left → current → right = alphabetical order for BST

void SymTable::print_all(SymNode* node) const {
    if (node == nullptr) return;
    print_all(node->left);
    std::cout << "  [slot " << node->mem_loc << "] "
              << node->key << " = " << node->value->to_string() << std::endl;
    print_all(node->right);
}