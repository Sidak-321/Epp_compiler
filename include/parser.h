// include/parser.h
// Tokenizer + recursive descent parser
// Takes a line of source code, builds an ExprTreeNode AST

#ifndef PARSER_H
#define PARSER_H

#include "exprtreenode.h"
#include "symtable.h"
#include "minheap.h"
#include <string>
#include <vector>

class Parser {
private:
    // Current token stream and position
    std::vector<std::string> tokens;
    int pos;  // current position in tokens

    // References to shared compiler state
    SymTable* symtable;
    MinHeap*  heap;

    // ── Tokenizer ─────────────────────────────────────────────────────────────
    // Splits "x = 3 + 5 * 2" into ["x","=","3","+","5","*","2"]
    std::vector<std::string> tokenize(const std::string& line);

    // ── Recursive Descent Parser ──────────────────────────────────────────────
    // Three levels handle operator precedence:
    //   parse_expr   → handles + and - (lowest precedence)
    //   parse_term   → handles * and / (higher precedence)
    //   parse_factor → handles numbers, variables, parentheses (atoms)

    ExprTreeNode* parse_expr();    // entry point for RHS parsing
    ExprTreeNode* parse_term();
    ExprTreeNode* parse_factor();

    // Peek at current token without consuming
    std::string peek_token();

    // Consume and return current token, advance pos
    std::string consume();

    // Check if string is a number (integer or rational like "1/3")
    bool is_number(const std::string& s);

public:
    Parser(SymTable* st, MinHeap* h);

    // Parse one line of source code
    // Returns the root ExprTreeNode of the AST
    ExprTreeNode* parse_line(const std::string& line);
};

#endif