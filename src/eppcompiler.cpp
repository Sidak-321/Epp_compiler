// src/eppcompiler.cpp

#include "eppcompiler.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

// ─── CONSTRUCTOR / DESTRUCTOR ─────────────────────────────────────────────────

EPPCompiler::EPPCompiler(int mem_size, const std::string& out_file) {
    memory_size = mem_size;
    output_file = out_file;
    heap        = new MinHeap(mem_size);
    parser      = new Parser(&symtable, heap);
}

EPPCompiler::~EPPCompiler() {
    delete parser;
    delete heap;
}

// ─── NEW TEMP ─────────────────────────────────────────────────────────────────
// Allocates a fresh memory slot for intermediate computation results

int EPPCompiler::new_temp() {
    int slot = heap->pop();
    temp_slots.push_back(slot);
    return slot;
}

// ─── WALK ─────────────────────────────────────────────────────────────────────
// Post-order AST traversal — generates instructions bottom up
// Returns the slot number where this node's result is stored

int EPPCompiler::walk(ExprTreeNode* node) {
    if (node == nullptr) return -1;

    // ── VAL node: load a literal value into a new slot ────────────────────────
    if (node->type == "VAL") {
        int slot = new_temp();
        std::string instr = "LOAD M[" + std::to_string(slot) + "] "
                          + node->value->to_string();
        instructions.push_back(instr);
        return slot;
    }

    // ── VAR node: look up variable's slot in symbol table ─────────────────────
    if (node->type == "VAR") {
        SymNode* entry = symtable.search(node->var_id);
        if (entry == nullptr) {
            throw std::runtime_error("Undefined variable: " + node->var_id);
        }
        return entry->mem_loc;
    }

    // ── DEL node: free the variable's slot ───────────────────────────────────
    if (node->type == "DEL") {
        SymNode* entry = symtable.search(node->var_id);
        if (entry != nullptr) {
            int slot = entry->mem_loc;
            instructions.push_back("FREE M[" + std::to_string(slot) + "]");
            symtable.remove(node->var_id);
            heap->push(slot);
        }
        return -1;
    }

    // ── RET node: return the variable's slot ──────────────────────────────────
    if (node->type == "RET") {
        SymNode* entry = symtable.search(node->var_id);
        if (entry == nullptr) {
            throw std::runtime_error("Undefined variable: " + node->var_id);
        }
        instructions.push_back("RET M[" + std::to_string(entry->mem_loc) + "]");
        return entry->mem_loc;
    }

    // ── Assignment: ADD node with VAR on left ─────────────────────────────────
    // Parser wraps assignments as ADD(VAR(x), rhs)
    if (node->type == "ADD" && node->left != nullptr
                             && node->left->type == "VAR") {
        // Check if this is an assignment (var not yet in symtable as result)
        // Walk the RHS to get its slot
        int rhs_slot = walk(node->right);

        // Update symtable entry to point to the RHS slot
        SymNode* entry = symtable.search(node->left->var_id);
        if (entry != nullptr) {
            // Free old slot if different
            if (entry->mem_loc != rhs_slot) {
                heap->push(entry->mem_loc);
            }
            entry->mem_loc = rhs_slot;
        }
        return rhs_slot;
    }

    // ── Operator nodes: ADD, SUB, MUL, DIV ───────────────────────────────────
    // Post-order: process left, then right, then emit instruction for this node
    int left_slot  = walk(node->left);
    int right_slot = walk(node->right);
    int result_slot = new_temp();

    std::string instr = node->type
                      + " M[" + std::to_string(result_slot) + "]"
                      + " M[" + std::to_string(left_slot)   + "]"
                      + " M[" + std::to_string(right_slot)  + "]";
    instructions.push_back(instr);
    return result_slot;
}

// ─── COMPILE ─────────────────────────────────────────────────────────────────

void EPPCompiler::compile(const std::vector<std::string>& lines) {
    std::cout << "[EPP] Compiling..." << std::endl;

    for (const std::string& line : lines) {
        if (line.empty()) continue;

        // Clear temp slots for each new line
        temp_slots.clear();

        // Parse line into AST
        ExprTreeNode* ast = parser->parse_line(line);
        if (ast == nullptr) continue;

        // Walk AST to generate instructions
        walk(ast);

        delete ast;
    }

    std::cout << "[EPP] Parsed " << lines.size() << " lines" << std::endl;
    std::cout << "[EPP] Generated " << instructions.size()
              << " instructions" << std::endl;
}

// ─── WRITE TO FILE ────────────────────────────────────────────────────────────

void EPPCompiler::write_to_file(const std::string& out_file) {
    std::ofstream file(out_file);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open output file: " + out_file);
    }
    for (const std::string& instr : instructions) {
        file << instr << "\n";
    }
    file.close();
    std::cout << "[EPP] Written to " << out_file << std::endl;
}

// ─── PRINT INSTRUCTIONS ───────────────────────────────────────────────────────

void EPPCompiler::print_instructions() const {
    for (const std::string& instr : instructions) {
        std::cout << "  " << instr << std::endl;
    }
}