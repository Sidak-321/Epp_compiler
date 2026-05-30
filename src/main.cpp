// src/main.cpp — Final CLI with compiler + interpreter + REPL

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "eppcompiler.h"
#include "interpreter.h"

// Read all lines from a file into a vector
std::vector<std::string> read_file(const std::string& path) {
    std::ifstream infile(path);
    if (!infile.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    return lines;
}

void print_usage() {
    std::cout << "EPP — Expression Processing Pipeline v1.0" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  Compile:     ./epp compile <input> <output> <memory>"
              << std::endl;
    std::cout << "  Interpret:   ./epp run <input>"
              << std::endl;
    std::cout << "  Interactive: ./epp repl"
              << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  ./epp compile program.txt output.txt 10" << std::endl;
    std::cout << "  ./epp run program.txt" << std::endl;
    std::cout << "  ./epp repl" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string mode = argv[1];

    // ── Mode 1: compile ───────────────────────────────────────────────────────
    if (mode == "compile") {
        if (argc != 5) {
            std::cout << "Usage: ./epp compile <input> <output> <memory>"
                      << std::endl;
            return 1;
        }
        std::string input_file  = argv[2];
        std::string output_file = argv[3];
        int memory_size         = std::stoi(argv[4]);

        std::cout << "[EPP] Compiling " << input_file << "..." << std::endl;
        std::cout << "[EPP] Memory limit: " << memory_size
                  << " slots" << std::endl;

        try {
            std::vector<std::string> lines = read_file(input_file);
            EPPCompiler compiler(memory_size, output_file);
            compiler.compile(lines);
            compiler.print_instructions();
            compiler.write_to_file(output_file);
        }
        catch (const std::exception& e) {
            std::cerr << "[EPP] Error: " << e.what() << std::endl;
            return 1;
        }
    }

    // ── Mode 2: run (interpret a file) ────────────────────────────────────────
    else if (mode == "run") {
        if (argc != 3) {
            std::cout << "Usage: ./epp run <input>" << std::endl;
            return 1;
        }
        std::string input_file = argv[2];
        std::cout << "[EPP] Running " << input_file << "..." << std::endl;

        try {
            std::vector<std::string> lines = read_file(input_file);
            Interpreter interp(100);  // 100 memory slots for interpreter
            interp.run_file(lines);
        }
        catch (const std::exception& e) {
            std::cerr << "[EPP] Error: " << e.what() << std::endl;
            return 1;
        }
    }

    // ── Mode 3: repl ──────────────────────────────────────────────────────────
    else if (mode == "repl") {
        Interpreter interp(100);
        interp.run_repl();
    }

    else {
        std::cout << "Unknown mode: " << mode << std::endl;
        print_usage();
        return 1;
    }

    return 0;
}