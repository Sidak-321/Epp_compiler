// src/main.cpp — Final CLI

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "eppcompiler.h"

int main(int argc, char* argv[]) {

    // ── CLI argument parsing ──────────────────────────────────────────────────
    if (argc != 4) {
        std::cout << "Usage: ./epp <input_file> <output_file> <memory_size>"
                  << std::endl;
        std::cout << "Example: ./epp program.txt output.txt 10" << std::endl;
        return 1;
    }
    

    std::string input_file  = argv[1];
    std::string output_file = argv[2];
    int memory_size         = std::stoi(argv[3]);

    std::cout << "[EPP] Expression Processing Pipeline v1.0" << std::endl;
    std::cout << "[EPP] Compiling " << input_file << "..." << std::endl;
    std::cout << "[EPP] Memory limit: " << memory_size << " slots" << std::endl;

    // ── Read input file ───────────────────────────────────────────────────────
    std::ifstream infile(input_file);
    if (!infile.is_open()) {
        std::cerr << "[EPP] Error: cannot open " << input_file << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    infile.close();

    // ── Compile ───────────────────────────────────────────────────────────────
    try {
        EPPCompiler compiler(memory_size, output_file);
        compiler.compile(lines);
        compiler.print_instructions();
        compiler.write_to_file(output_file);
    }
    catch (const std::exception& e) {
        std::cerr << "[EPP] Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}