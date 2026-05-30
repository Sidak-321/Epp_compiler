// src/interpreter.cpp

#include "interpreter.h"
#include <iostream>
#include <stdexcept>

// ─── CONSTRUCTOR / DESTRUCTOR ─────────────────────────────────────────────────

Interpreter::Interpreter(int memory_size) {
    heap   = new MinHeap(memory_size);
    parser = new Parser(&symtable, heap);
}

Interpreter::~Interpreter() {
    delete parser;
    delete heap;
}

// ─── EVALUATE ─────────────────────────────────────────────────────────────────
// The core of the interpreter — walks AST, returns computed value
// Post-order: evaluate children first, then combine at current node

UnlimitedRational* Interpreter::evaluate(ExprTreeNode* node) {
    if (node == nullptr) return nullptr;

    // ── VAL: return the literal value ─────────────────────────────────────────
    if (node->type == "VAL") {
        return new UnlimitedRational(*node->value);
    }

    // ── VAR: look up variable in symbol table ─────────────────────────────────
    if (node->type == "VAR") {
        SymNode* entry = symtable.search(node->var_id);
        if (entry == nullptr) {
            throw std::runtime_error("Undefined variable: " + node->var_id);
        }
        return new UnlimitedRational(*entry->value);
    }

    // ── DEL: remove variable from symbol table ────────────────────────────────
    if (node->type == "DEL") {
        SymNode* entry = symtable.search(node->var_id);
        if (entry != nullptr) {
            int slot = entry->mem_loc;
            symtable.remove(node->var_id);
            heap->push(slot);
        }
        return nullptr;  // del produces no value
    }

    // ── RET: return variable's value ──────────────────────────────────────────
    if (node->type == "RET") {
        SymNode* entry = symtable.search(node->var_id);
        if (entry == nullptr) {
            throw std::runtime_error("Undefined variable: " + node->var_id);
        }
        return new UnlimitedRational(*entry->value);
    }

    // ── Assignment: ADD node with VAR on left ─────────────────────────────────
    // Parser wraps x = expr as ADD(VAR(x), expr)
    if (node->type == "ADD" && node->left != nullptr
                             && node->left->type == "VAR") {
        // Evaluate the RHS
        UnlimitedRational* rhs = evaluate(node->right);

        std::string var_name = node->left->var_id;
        SymNode* existing = symtable.search(var_name);

        if (existing != nullptr) {
            // Variable already exists — update its value
            delete existing->value;
            existing->value = new UnlimitedRational(*rhs);
        }
        // If not existing, parser already inserted it with placeholder
        // Update the value now
        existing = symtable.search(var_name);
        if (existing != nullptr) {
            delete existing->value;
            existing->value = new UnlimitedRational(*rhs);
        }

        delete rhs;
        return nullptr;  // assignment produces no printable output
    }

    // ── Operator nodes: evaluate both sides then apply operation ──────────────
    UnlimitedRational* left  = evaluate(node->left);
    UnlimitedRational* right = evaluate(node->right);

    UnlimitedRational* result = nullptr;

    if      (node->type == "ADD") result = UnlimitedRational::add(left, right);
    else if (node->type == "SUB") result = UnlimitedRational::sub(left, right);
    else if (node->type == "MUL") result = UnlimitedRational::mul(left, right);
    else if (node->type == "DIV") result = UnlimitedRational::div(left, right);
    else throw std::runtime_error("Unknown node type: " + node->type);

    delete left;
    delete right;
    return result;
}

// ─── RUN LINE ─────────────────────────────────────────────────────────────────
// Parse and evaluate one line
// Returns result string if line produces output, "" otherwise

std::string Interpreter::run_line(const std::string& line) {
    if (line.empty()) return "";

    ExprTreeNode* ast = parser->parse_line(line);
    if (ast == nullptr) return "";

    UnlimitedRational* result = evaluate(ast);
    delete ast;

    if (result != nullptr) {
        std::string out = result->to_string();
        delete result;
        return out;
    }
    return "";
}

// ─── RUN FILE ─────────────────────────────────────────────────────────────────
// Run all lines from a file, print ret results

void Interpreter::run_file(const std::vector<std::string>& lines) {
    for (const std::string& line : lines) {
        if (line.empty()) continue;
        try {
            std::string result = run_line(line);
            if (!result.empty()) {
                std::cout << "=> " << result << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[EPP] Runtime error: " << e.what() << std::endl;
        }
    }
}

// ─── REPL ─────────────────────────────────────────────────────────────────────
// Interactive Read-Eval-Print Loop

void Interpreter::run_repl() {
    std::cout << "EPP Interpreter v1.0" << std::endl;
    std::cout << "Type expressions to evaluate. 'exit' to quit." << std::endl;
    std::cout << std::endl;

    std::string line;
    while (true) {
        std::cout << "EPP> ";
        std::getline(std::cin, line);

        // Trim whitespace
        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) continue;
        line = line.substr(start);

        if (line == "exit" || line == "quit") {
            std::cout << "Goodbye!" << std::endl;
            break;
        }

        try {
            std::string result = run_line(line);
            if (!result.empty()) {
                std::cout << "=> " << result << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}