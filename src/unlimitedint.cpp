// src/unlimitedint.cpp

#include "unlimitedint.h"
#include <string>
#include <algorithm>  // for std::max

// ─── CONSTRUCTORS ────────────────────────────────────────────────────────────

// Build from a string like "12345" or "-99"
UnlimitedInt::UnlimitedInt(std::string num) {
    if (num.empty() || num == "0") {
        sign = 0; size = 1;
        digits = new int[1];
        digits[0] = 0;
        return;
    }

    // Check for negative sign
    int start = 0;
    if (num[0] == '-') { sign = -1; start = 1; }
    else               { sign = +1; }

    // Strip the sign character, keep only digit characters
    std::string d = num.substr(start);
    size = d.size();
    digits = new int[size];

    // Store in reverse: "123" → digits[0]=3, digits[1]=2, digits[2]=1
    for (int i = 0; i < size; i++) {
        digits[i] = d[size - 1 - i] - '0';  // '3' - '0' = 3
    }
}

// Build from a plain int
UnlimitedInt::UnlimitedInt(int num) {
    // Reuse string constructor
    *this = UnlimitedInt(std::to_string(num));
}

// Build from raw array (used internally by arithmetic functions)
UnlimitedInt::UnlimitedInt(int* arr, int sz, int sg) {
    size = sz;
    sign = sg;
    digits = new int[size];
    for (int i = 0; i < size; i++) digits[i] = arr[i];
}

// Copy constructor
// Copy constructor
UnlimitedInt::UnlimitedInt(const UnlimitedInt& other) {
    size = other.size;
    sign = other.sign;
    digits = new int[size];
    for (int i = 0; i < size; i++) digits[i] = other.digits[i];
}

// Assignment operator
UnlimitedInt& UnlimitedInt::operator=(const UnlimitedInt& other) {
    if (this == &other) return *this;  // guard against self-assignment: x = x
    delete[] digits;                   // free existing memory first
    size = other.size;
    sign = other.sign;
    digits = new int[size];
    for (int i = 0; i < size; i++) digits[i] = other.digits[i];
    return *this;
}

// Destructor
UnlimitedInt::~UnlimitedInt() {
    delete[] digits;
}
//setters
void UnlimitedInt::set_sign(int s) { sign = s; }

// ─── GETTERS ─────────────────────────────────────────────────────────────────

int  UnlimitedInt::get_sign()   const { return sign; }
int  UnlimitedInt::get_size()   const { return size; }
int* UnlimitedInt::get_digits() const { return digits; }

// ─── TO STRING ───────────────────────────────────────────────────────────────

std::string UnlimitedInt::to_string() const {
    if (sign == 0) return "0";
    std::string result = "";
    if (sign == -1) result = "-";
    // digits are stored in reverse, so read from end to start
    for (int i = size - 1; i >= 0; i--) {
        result += ('0' + digits[i]);
    }
    return result;
}

// ─── COMPARE ─────────────────────────────────────────────────────────────────
// Returns: -1 if a < b, 0 if a == b, +1 if a > b

int UnlimitedInt::compare(UnlimitedInt* a, UnlimitedInt* b) {
    // Different signs
    if (a->sign > b->sign) return  1;
    if (a->sign < b->sign) return -1;
    if (a->sign == 0)      return  0;  // both zero

    // Same sign — compare magnitudes
    int cmp = 0;
    if (a->size > b->size) cmp =  1;
    else if (a->size < b->size) cmp = -1;
    else {
        // Same length — compare digit by digit from most significant
        for (int i = a->size - 1; i >= 0; i--) {
            if (a->digits[i] > b->digits[i]) { cmp =  1; break; }
            if (a->digits[i] < b->digits[i]) { cmp = -1; break; }
        }
    }
    // If both negative, flip the result (larger magnitude = smaller number)
    return (a->sign == -1) ? -cmp : cmp;
}

// ─── ADDITION (magnitude only, same sign assumed) ────────────────────────────
// Internal helper — adds two positive arrays
static UnlimitedInt* add_magnitudes(UnlimitedInt* a, UnlimitedInt* b) {
    int maxLen = std::max(a->get_size(), b->get_size());
    int* result = new int[maxLen + 1]();  // +1 for possible carry, () zero-inits
    int carry = 0;

    for (int i = 0; i < maxLen || carry; i++) {
        int da = (i < a->get_size()) ? a->get_digits()[i] : 0;
        int db = (i < b->get_size()) ? b->get_digits()[i] : 0;
        int sum = da + db + carry;
        result[i] = sum % 10;
        carry = sum / 10;
    }

    // Find actual size (trim leading zeros)
    int sz = maxLen + 1;
    while (sz > 1 && result[sz-1] == 0) sz--;

    UnlimitedInt* out = new UnlimitedInt(result, sz, 1);
    delete[] result;
    return out;
}

// Internal helper — subtracts smaller magnitude from larger (a >= b assumed)
static UnlimitedInt* sub_magnitudes(UnlimitedInt* a, UnlimitedInt* b) {
    int* result = new int[a->get_size()]();
    int borrow = 0;

    for (int i = 0; i < a->get_size(); i++) {
        int da = a->get_digits()[i];
        int db = (i < b->get_size()) ? b->get_digits()[i] : 0;
        int diff = da - db - borrow;
        if (diff < 0) { diff += 10; borrow = 1; }
        else            borrow = 0;
        result[i] = diff;
    }

    // Trim leading zeros
    int sz = a->get_size();
    while (sz > 1 && result[sz-1] == 0) sz--;

    int sg = (sz == 1 && result[0] == 0) ? 0 : 1;
    UnlimitedInt* out = new UnlimitedInt(result, sz, sg);
    delete[] result;
    return out;
}

// ─── ADD ─────────────────────────────────────────────────────────────────────

UnlimitedInt* UnlimitedInt::add(UnlimitedInt* a, UnlimitedInt* b) {
    if (a->sign == 0) return new UnlimitedInt(*b);
    if (b->sign == 0) return new UnlimitedInt(*a);

    if (a->sign == b->sign) {
        // Same sign: add magnitudes, keep sign
        UnlimitedInt* res = add_magnitudes(a, b);
        res->sign = a->sign;
        return res;
    }

    // Different signs: subtract smaller from larger
    // Compare magnitudes (ignore sign)
    UnlimitedInt tempA(*a); tempA.sign = 1;
    UnlimitedInt tempB(*b); tempB.sign = 1;
    int cmp = compare(&tempA, &tempB);

    if (cmp == 0) return new UnlimitedInt(0);  // they cancel out

    UnlimitedInt* res;
    if (cmp > 0) {
        res = sub_magnitudes(a, b);
        res->sign = a->sign;
    } else {
        res = sub_magnitudes(b, a);
        res->sign = b->sign;
    }
    return res;
}

// ─── SUBTRACT ────────────────────────────────────────────────────────────────

UnlimitedInt* UnlimitedInt::sub(UnlimitedInt* a, UnlimitedInt* b) {
    // a - b = a + (-b)
    UnlimitedInt negB(*b);
    negB.sign = -b->sign;
    return add(a, &negB);
}

// ─── MULTIPLY ────────────────────────────────────────────────────────────────

UnlimitedInt* UnlimitedInt::mul(UnlimitedInt* a, UnlimitedInt* b) {
    if (a->sign == 0 || b->sign == 0) return new UnlimitedInt(0);

    int n = a->size, m = b->size;
    int* result = new int[n + m]();  // zero-initialized

    // Grade school multiplication
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            result[i + j] += a->digits[i] * b->digits[j];
            // Propagate carry
            if (result[i + j] >= 10) {
                result[i + j + 1] += result[i + j] / 10;
                result[i + j] %= 10;
            }
        }
    }

    int sz = n + m;
    while (sz > 1 && result[sz-1] == 0) sz--;

    int sg = (a->sign == b->sign) ? 1 : -1;
    if (sz == 1 && result[0] == 0) sg = 0;

    UnlimitedInt* out = new UnlimitedInt(result, sz, sg);
    delete[] result;
    return out;
}

// ─── DIVIDE ──────────────────────────────────────────────────────────────────
// ─── DIVIDE ──────────────────────────────────────────────────────────────────

// Internal helper: compare magnitudes only (ignore sign)
static int compare_magnitude(UnlimitedInt* a, UnlimitedInt* b) {
    if (a->get_size() > b->get_size()) return  1;
    if (a->get_size() < b->get_size()) return -1;
    for (int i = a->get_size() - 1; i >= 0; i--) {
        if (a->get_digits()[i] > b->get_digits()[i]) return  1;
        if (a->get_digits()[i] < b->get_digits()[i]) return -1;
    }
    return 0;
}

// Internal helper: multiply magnitude by single digit (0-9)
static UnlimitedInt* mul_single(UnlimitedInt* a, int d) {
    int* result = new int[a->get_size() + 1]();
    int carry = 0;
    for (int i = 0; i < a->get_size(); i++) {
        int val = a->get_digits()[i] * d + carry;
        result[i] = val % 10;
        carry = val / 10;
    }
    int sz = a->get_size() + 1;
    if (carry) result[a->get_size()] = carry;
    while (sz > 1 && result[sz-1] == 0) sz--;
    int sg = (sz == 1 && result[0] == 0) ? 0 : 1;
    UnlimitedInt* out = new UnlimitedInt(result, sz, sg);
    delete[] result;
    return out;
}

UnlimitedInt* UnlimitedInt::div(UnlimitedInt* a, UnlimitedInt* b) {
    // Work with magnitudes, fix sign at the end
    UnlimitedInt absA(*a); absA.sign = (a->sign == 0) ? 0 : 1;
    UnlimitedInt absB(*b); absB.sign = (b->sign == 0) ? 0 : 1;

    // If a < b, result is 0
    if (compare_magnitude(&absA, &absB) < 0) return new UnlimitedInt(0);

    // Long division digit by digit
    int n = absA.get_size();
    int* quotDigits = new int[n]();  // result digits (same order as input)
    
    // 'current' holds the running remainder as we bring down digits
    // We work most-significant digit first (index n-1 down to 0)
    int* curArr = new int[n]();
    int curSize = 1;  // start with 0

    for (int i = n - 1; i >= 0; i--) {
        // Shift current left by 1 (multiply by 10) and bring down next digit
        // Shift: move all digits up one position
        for (int j = curSize; j > 0; j--) curArr[j] = curArr[j-1];
        curArr[0] = absA.get_digits()[i];  // bring down digit
        if (curSize < n) curSize++;

        // Trim curSize to actual size
        while (curSize > 1 && curArr[curSize-1] == 0) curSize--;

        UnlimitedInt cur(curArr, curSize, (curSize==1 && curArr[0]==0) ? 0 : 1);

        // Find largest q (0-9) such that q * absB <= cur
        int q = 0;
        for (int d = 9; d >= 1; d--) {
            UnlimitedInt* prod = mul_single(&absB, d);
            if (compare_magnitude(prod, &cur) <= 0) {
                q = d;
                delete prod;
                break;
            }
            delete prod;
        }

        quotDigits[i] = q;  // store in same index as input digit

        // current = current - q * absB
        if (q > 0) {
            UnlimitedInt* qb = mul_single(&absB, q);
            UnlimitedInt* rem = sub_magnitudes(&cur, qb);
            // Copy rem back into curArr
            curSize = rem->get_size();
            for (int j = 0; j < curSize; j++) curArr[j] = rem->get_digits()[j];
            for (int j = curSize; j < n; j++) curArr[j] = 0;
            delete qb;
            delete rem;
        }
    }

    // quotDigits is in reverse order (index 0 = ones place) — already correct
    // for UnlimitedInt internal format
    int sz = n;
    while (sz > 1 && quotDigits[sz-1] == 0) sz--;
    int sg = (sz == 1 && quotDigits[0] == 0) ? 0 : 1;
    if (sg != 0) sg = (a->sign == b->sign) ? 1 : -1;

    UnlimitedInt* result = new UnlimitedInt(quotDigits, sz, sg);
    delete[] quotDigits;
    delete[] curArr;
    return result;
}


// ─── MODULO ──────────────────────────────────────────────────────────────────

UnlimitedInt* UnlimitedInt::mod(UnlimitedInt* a, UnlimitedInt* b) {
    UnlimitedInt* q   = div(a, b);
    UnlimitedInt* qb  = mul(q, b);
    UnlimitedInt* rem = sub(a, qb);
    if (rem->get_sign() == -1) {
        UnlimitedInt* fixed = add(rem, b);
        delete rem; rem = fixed;
    }
    delete q;
    delete qb;
    return rem;
}

// ─── GCD (Euclidean algorithm) ───────────────────────────────────────────────

UnlimitedInt* UnlimitedInt::gcd(UnlimitedInt* a, UnlimitedInt* b) {
    UnlimitedInt x(*a); x.sign = (a->sign == 0) ? 0 : 1;
    UnlimitedInt y(*b); y.sign = (b->sign == 0) ? 0 : 1;

    while (y.sign != 0) {
        UnlimitedInt* r = mod(&x, &y);
        x = y;
        y = *r;
        delete r;
    }
    return new UnlimitedInt(x);
}

