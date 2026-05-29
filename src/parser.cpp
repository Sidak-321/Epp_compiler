// src/parser.cpp

#include "parser.h"
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iostream>

// ─── CONSTRUCTOR ─────────────────────────────────────────────────────────────

Parser::Parser(SymTable* st, MinHeap* h) {
    symtable = st;
    heap     = h;
    pos      = 0;
}

// ─── TOKENIZER ────────────────────────────────────────────────────────────────
// Splits "x = 3 + 5 * 2" into ["x","=","3","+","5","*","2"]

std::vector<std::string> Parser::tokenize(const std::string& line) {
    std::vector<std::string> result;
    std::istringstream stream(line);
    std::string token;
    while (stream >> token) {
        result.push_back(token);
    }
    return result;
}

// ─── HELPERS ─────────────────────────────────────────────────────────────────

std::string Parser::peek_token() {
    if (pos >= (int)tokens.size()) return "";
    return tokens[pos];
}

std::string Parser::consume() {
    if (pos >= (int)tokens.size()) return "";
    return tokens[pos++];
}

// Check if a string is a numeric literal: "3", "42", "0"
bool Parser::is_number(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

// ─── PARSE FACTOR ─────────────────────────────────────────────────────────────
// Atoms: numbers → VAL node, variable names → VAR node

ExprTreeNode* Parser::parse_factor() {
    std::string tok = consume();

    if (is_number(tok)) {
        UnlimitedRational* val = new UnlimitedRational(tok, "1");
        ExprTreeNode* node = new ExprTreeNode(val);
        delete val;
        return node;
    }
    else {
        return new ExprTreeNode("VAR", tok);
    }
}

// ─── PARSE TERM ───────────────────────────────────────────────────────────────
// Handles * and / (higher precedence)
// Calls parse_factor for each operand

ExprTreeNode* Parser::parse_term() {
    ExprTreeNode* left = parse_factor();

    while (peek_token() == "*" || peek_token() == "/") {
        std::string op    = consume();
        ExprTreeNode* right = parse_factor();
        std::string type  = (op == "*") ? "MUL" : "DIV";
        left = new ExprTreeNode(type, left, right);
    }

    return left;
}

// ─── PARSE EXPR ───────────────────────────────────────────────────────────────
// Handles + and - (lower precedence)
// Calls parse_term so * and / always go deeper in the tree

ExprTreeNode* Parser::parse_expr() {
    ExprTreeNode* left = parse_term();

    while (peek_token() == "+" || peek_token() == "-") {
        std::string op    = consume();
        ExprTreeNode* right = parse_term();
        std::string type  = (op == "+") ? "ADD" : "SUB";
        left = new ExprTreeNode(type, left, right);
    }

    return left;
}

// ─── PARSE LINE ───────────────────────────────────────────────────────────────
// Main entry point — identifies statement type and dispatches

ExprTreeNode* Parser::parse_line(const std::string& line) {
    tokens = tokenize(line);
    pos    = 0;

    if (tokens.empty()) return nullptr;

    // ── Case 1: del x ─────────────────────────────────────────────────────────
    if (tokens[0] == "del") {
        std::string var_name = tokens[1];
        int freed_slot = symtable->remove(var_name);
        if (freed_slot >= 0) {
            heap->push(freed_slot);  // return slot back to heap
        }
        return new ExprTreeNode("DEL", var_name);
    }

    // ── Case 2: ret x ─────────────────────────────────────────────────────────
    if (tokens[0] == "ret") {
        std::string var_name = tokens[1];
        return new ExprTreeNode("RET", var_name);
    }

    // ── Case 3: x = expr ──────────────────────────────────────────────────────
    if (tokens.size() >= 3 && tokens[1] == "=") {
        std::string var_name = tokens[0];

        // Pop slot and insert into symtable FIRST
        // before any UnlimitedRational is created in parse_expr
        int slot = heap->pop();
        UnlimitedRational placeholder("0", "1");
        symtable->insert(var_name, &placeholder, slot);

        // Now parse the RHS
        pos = 2;
        ExprTreeNode* rhs = parse_expr();

        // Wrap in assignment node: left=VAR(name), right=rhs
        ExprTreeNode* var_node = new ExprTreeNode("VAR", var_name);
        return new ExprTreeNode("ADD", var_node, rhs);
    }

    return nullptr;
}