#include <iostream>
#include "unlimitedint.h"

int main() {
    std::cout << "Step 1: Creating a..." << std::endl;
    UnlimitedInt a("123");
    std::cout << "a = " << a.to_string() << std::endl;

    std::cout << "Step 2: Creating b..." << std::endl;
    UnlimitedInt b("456");
    std::cout << "b = " << b.to_string() << std::endl;

    std::cout << "Step 3: Adding..." << std::endl;
    UnlimitedInt* sum = UnlimitedInt::add(&a, &b);
    std::cout << "123 + 456 = " << sum->to_string() << std::endl;
    delete sum;

    std::cout << "Step 4: Subtracting..." << std::endl;
    UnlimitedInt* diff = UnlimitedInt::sub(&b, &a);
    std::cout << "456 - 123 = " << diff->to_string() << std::endl;
    delete diff;

    std::cout << "Step 5: Multiplying..." << std::endl;
    UnlimitedInt* prod = UnlimitedInt::mul(&a, &b);
    std::cout << "123 * 456 = " << prod->to_string() << std::endl;
    delete prod;

    std::cout << "Step 6: Dividing..." << std::endl;
    UnlimitedInt* quot = UnlimitedInt::div(&b, &a);
    std::cout << "456 / 123 = " << quot->to_string() << std::endl;
    delete quot;

    std::cout << "All steps done." << std::endl;
    return 0;
}