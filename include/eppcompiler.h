// include/eppcompiler.h
// The main compiler class — ties everything together

#ifndef EPPCOMPILER_H
#define EPPCOMPILER_H

#include "parser.h"
#include "symtable.h"
#include "minheap.h"
#include "exprtreenode.h"
#include <string>
#include <vector>

class EPPCompiler {
private:
    SymTable  symtable;
    MinHeap*  heap;
    Parser*   parser;

    // All generated instructions stored here
    std::vector<std::string> instructions;

    // Tracks which AST slot each result is in
    // Returns the slot number where the result of this node lives
    int walk(ExprTreeNode* node);

    // Allocate a fresh temp slot for intermediate results
    int new_temp();

    // Track temp slots so we can free them after use
    std::vector<int> temp_slots;

public:
    EPPCompiler(int memory_size, const std::string& output_file);
    ~EPPCompiler();

    // Process all lines of source code
    void compile(const std::vector<std::string>& lines);

    // Write instructions to output file
    void write_to_file(const std::string& output_file);

    // Print instructions to console
    void print_instructions() const;

private:
    std::string output_file;
    int         memory_size;
};

#endif