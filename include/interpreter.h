// include/interpreter.h
// Tree-walking interpreter — evaluates AST nodes directly

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "exprtreenode.h"
#include "symtable.h"
#include "minheap.h"
#include "parser.h"
#include <string>

class Interpreter {
private:
    SymTable  symtable;
    MinHeap*  heap;
    Parser*   parser;

    // Walk the AST and return the computed value
    // Returns nullptr for statements that don't produce a value (del)
    UnlimitedRational* evaluate(ExprTreeNode* node);

public:
    Interpreter(int memory_size);
    ~Interpreter();

    // Run a single line — returns result string or "" if no output
    std::string run_line(const std::string& line);

    // Run a whole file
    void run_file(const std::vector<std::string>& lines);

    // Start interactive REPL loop
    void run_repl();
};

#endif