// include/exprtreenode.h
// A single node in the Abstract Syntax Tree (AST)

#ifndef EXPRTREENODE_H
#define EXPRTREENODE_H

#include "unlimitedrational.h"
#include <string>

class ExprTreeNode {
public:
    // ── What kind of node is this? ──────────────────────────────────────────
    std::string type;
    // Possible values:
    //   "ADD", "SUB", "MUL", "DIV"  → operator nodes
    //   "VAL"                        → number literal
    //   "VAR"                        → variable name
    //   "DEL"                        → delete instruction
    //   "RET"                        → return instruction

    // ── Node data (only one is used depending on type) ──────────────────────
    UnlimitedRational* value;   // used when type == "VAL"
    std::string        var_id;  // used when type == "VAR", "DEL", "RET"

    // ── Tree structure ───────────────────────────────────────────────────────
    ExprTreeNode* left;    // left child  (used by operator nodes)
    ExprTreeNode* right;   // right child (used by operator nodes)

    // ── Constructors ─────────────────────────────────────────────────────────

    // For operator nodes: ADD, SUB, MUL, DIV
    // Takes the type and its two children
    ExprTreeNode(std::string node_type,
                 ExprTreeNode* left_child,
                 ExprTreeNode* right_child);

    // For VAL nodes: a numeric value
    ExprTreeNode(UnlimitedRational* val);

    // For VAR, DEL, RET nodes: just a variable name string
    ExprTreeNode(std::string node_type, std::string id);

    // ── Destructor ────────────────────────────────────────────────────────────
    // Recursively deletes the entire subtree
    ~ExprTreeNode();

    // ── Utility ───────────────────────────────────────────────────────────────
    // Print the tree in a readable way (for debugging)
    void print(int indent = 0) const;
};

#endif