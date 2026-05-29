// src/main.cpp — Phase 4 test

#include <iostream>
#include "symtable.h"

int main() {
    std::cout << "=== Phase 4: Symbol Table (BST) ===" << std::endl;

    SymTable table;

    // ── Insert some variables ─────────────────────────────────────────────────
    // Simulating: x=10, y=20, z=30, a=5
    // BST will order them alphabetically: a, x, y, z

    UnlimitedRational v10("10", "1");
    UnlimitedRational v20("20", "1");
    UnlimitedRational v30("30", "1");
    UnlimitedRational v5("5",  "1");

    table.insert("x", &v10, 0);   // x at memory slot 0
    table.insert("y", &v20, 1);   // y at memory slot 1
    table.insert("z", &v30, 2);   // z at memory slot 2
    table.insert("a", &v5,  3);   // a at memory slot 3

    std::cout << "\nAfter inserting x, y, z, a:" << std::endl;
    table.print_all(table.search("x") ? nullptr : nullptr);  // trick: use root
    // Direct print from root:
    std::cout << "Size = " << table.get_size() << std::endl;

    // ── Search ────────────────────────────────────────────────────────────────
    std::cout << "\nSearch for 'y':" << std::endl;
    SymNode* found = table.search("y");
    if (found) {
        std::cout << "  Found: y = " << found->value->to_string()
                  << " at slot " << found->mem_loc << std::endl;
    }

    std::cout << "\nSearch for 'b' (not inserted):" << std::endl;
    SymNode* notFound = table.search("b");
    std::cout << (notFound ? "  Found" : "  Not found") << std::endl;

    // ── Remove ────────────────────────────────────────────────────────────────
    std::cout << "\nRemoving 'x'..." << std::endl;
    int freed = table.remove("x");
    std::cout << "  Freed memory slot: " << freed << std::endl;
    std::cout << "  Size after remove: " << table.get_size() << std::endl;

    // Search for x after deletion
    SymNode* afterDel = table.search("x");
    std::cout << "  Search x after delete: "
              << (afterDel ? "still there (BUG!)" : "gone (correct)") << std::endl;

    // ── Remove node with two children ─────────────────────────────────────────
    std::cout << "\nRemoving 'y' (has right child z)..." << std::endl;
    int freed2 = table.remove("y");
    std::cout << "  Freed slot: " << freed2 << std::endl;
    std::cout << "  Size: " << table.get_size() << std::endl;

    std::cout << "\n[EPP] Phase 4 complete." << std::endl;
    return 0;
}