// include/unlimitedrational.h
// Exact fraction: numerator / denominator, always in reduced form

#ifndef UNLIMITEDRATIONAL_H
#define UNLIMITEDRATIONAL_H

#include "unlimitedint.h"
#include <string>

class UnlimitedRational {
private:
    UnlimitedInt* num;   // numerator
    UnlimitedInt* den;   // denominator (always positive, never zero)

public:
    // Constructors
    UnlimitedRational(UnlimitedInt* numerator, UnlimitedInt* denominator);
    UnlimitedRational(std::string n, std::string d);  // from strings

    // Copy constructor
    UnlimitedRational(const UnlimitedRational& other);
    // Assignment operator
UnlimitedRational& operator=(const UnlimitedRational& other);

    // Destructor
    ~UnlimitedRational();

    // Getters
    UnlimitedInt* get_num() const;
    UnlimitedInt* get_den() const;

    // Reduce to lowest terms: 6/4 → 3/2
    void simplify();

    // Arithmetic — return new pointer (caller deletes)
    static UnlimitedRational* add(UnlimitedRational* a, UnlimitedRational* b);
    static UnlimitedRational* sub(UnlimitedRational* a, UnlimitedRational* b);
    static UnlimitedRational* mul(UnlimitedRational* a, UnlimitedRational* b);
    static UnlimitedRational* div(UnlimitedRational* a, UnlimitedRational* b);

    // "3/2" or "5" if denominator is 1
    std::string to_string() const;
};

#endif