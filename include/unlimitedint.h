// include/unlimitedint.h
// Stores an integer of arbitrary size as a dynamic array of digits

#ifndef UNLIMITEDINT_H
#define UNLIMITEDINT_H

#include <string>

class UnlimitedInt {
private:
    int* digits;   // array of digits, stored in reverse (index 0 = ones place)
    int  size;     // how many digits
    int  sign;     // +1 for positive, -1 for negative, 0 for zero

public:
    // Constructors
    UnlimitedInt(std::string num);   // from string: "12345" or "-12345"
    UnlimitedInt(int num);           // from int: 42
    UnlimitedInt(int* arr, int sz, int sg); // from raw array (internal use)

    // Copy constructor — needed when returning by value
    UnlimitedInt(const UnlimitedInt& other);
    // Assignment operator — needed for y = *r style assignments
     UnlimitedInt& operator=(const UnlimitedInt& other);

    // Destructor — frees the digits array
    ~UnlimitedInt();
    //setters
    void set_sign(int s);

    // Getters
    int  get_sign()          const;
    int  get_size()          const;
    int* get_digits()        const;


    // Arithmetic — static: take two pointers, return new pointer
    // Caller is responsible for deleting the returned pointer
    static UnlimitedInt* add(UnlimitedInt* a, UnlimitedInt* b);
    static UnlimitedInt* sub(UnlimitedInt* a, UnlimitedInt* b);
    static UnlimitedInt* mul(UnlimitedInt* a, UnlimitedInt* b);
    static UnlimitedInt* div(UnlimitedInt* a, UnlimitedInt* b);
    static UnlimitedInt* mod(UnlimitedInt* a, UnlimitedInt* b);
    static UnlimitedInt* gcd(UnlimitedInt* a, UnlimitedInt* b);

    // Comparison — returns -1, 0, or 1
    static int compare(UnlimitedInt* a, UnlimitedInt* b);

    // Convert back to string
    std::string to_string() const;
};

#endif