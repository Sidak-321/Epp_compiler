// src/unlimitedrational.cpp

#include "unlimitedrational.h"

// ─── CONSTRUCTORS ────────────────────────────────────────────────────────────

UnlimitedRational::UnlimitedRational(UnlimitedInt* numerator, UnlimitedInt* denominator) {
    num = new UnlimitedInt(*numerator);
    den = new UnlimitedInt(*denominator);
    simplify();
}

UnlimitedRational::UnlimitedRational(std::string n, std::string d) {
    num = new UnlimitedInt(n);
    den = new UnlimitedInt(d);
    simplify();
}

UnlimitedRational::UnlimitedRational(const UnlimitedRational& other) {
    num = new UnlimitedInt(*other.num);
    den = new UnlimitedInt(*other.den);
}

UnlimitedRational::~UnlimitedRational() {
    delete num;
    delete den;
}
// Assignment operator
UnlimitedRational& UnlimitedRational::operator=(const UnlimitedRational& other) {
    if (this == &other) return *this;
    delete num;
    delete den;
    num = new UnlimitedInt(*other.num);
    den = new UnlimitedInt(*other.den);
    return *this;
}

// ─── GETTERS ─────────────────────────────────────────────────────────────────

UnlimitedInt* UnlimitedRational::get_num() const { return num; }
UnlimitedInt* UnlimitedRational::get_den() const { return den; }

// ─── SIMPLIFY ────────────────────────────────────────────────────────────────
// Divides numerator and denominator by their GCD
// e.g. 6/4 → gcd(6,4)=2 → 3/2

void UnlimitedRational::simplify() {
    // If numerator is zero, set to 0/1 and done
    if (num->get_sign() == 0) {
        delete den;
        den = new UnlimitedInt(1);
        return;
    }

    // If denominator is already 1, nothing to simplify
    if (den->to_string() == "1" || den->to_string() == "-1") {
        if (den->get_sign() == -1) {
            // flip sign to numerator
            UnlimitedInt negOne(-1);
            UnlimitedInt* newNum = UnlimitedInt::mul(num, &negOne);
            delete num;
            num = newNum;
            delete den;
            den = new UnlimitedInt(1);
        }
        return;
    }

    // Make copies with positive signs for GCD computation
    UnlimitedInt posNum(*num);
    if (posNum.get_sign() == -1) {
        UnlimitedInt negOne(-1);
        UnlimitedInt* tmp = UnlimitedInt::mul(&posNum, &negOne);
        posNum = *tmp;
        delete tmp;
    }

    UnlimitedInt posDen(*den);
    if (posDen.get_sign() == -1) {
        UnlimitedInt negOne(-1);
        UnlimitedInt* tmp = UnlimitedInt::mul(&posDen, &negOne);
        posDen = *tmp;
        delete tmp;
    }

    // Compute GCD of absolute values
    UnlimitedInt* g = UnlimitedInt::gcd(&posNum, &posDen);

    // Divide both by GCD
    UnlimitedInt* newNum = UnlimitedInt::div(num, g);
    UnlimitedInt* newDen = UnlimitedInt::div(den, g);
    delete g;
    delete num;
    delete den;
    num = newNum;
    den = newDen;

    // Denominator must always be positive — move sign to numerator
    if (den->get_sign() == -1) {
        UnlimitedInt negOne(-1);
        UnlimitedInt* fn = UnlimitedInt::mul(num, &negOne);
        UnlimitedInt* fd = UnlimitedInt::mul(den, &negOne);
        delete num;
        delete den;
        num = fn;
        den = fd;
    }
}

// ─── TO STRING ───────────────────────────────────────────────────────────────

std::string UnlimitedRational::to_string() const {
    if (den->to_string() == "1") return num->to_string();
    return num->to_string() + "/" + den->to_string();
}

// ─── ADD ─────────────────────────────────────────────────────────────────────
// a/b + c/d = (a*d + c*b) / (b*d)

UnlimitedRational* UnlimitedRational::add(UnlimitedRational* a, UnlimitedRational* b) {
    UnlimitedInt* ad  = UnlimitedInt::mul(a->num, b->den);
    UnlimitedInt* cb  = UnlimitedInt::mul(b->num, a->den);
    UnlimitedInt* top = UnlimitedInt::add(ad, cb);
    UnlimitedInt* bot = UnlimitedInt::mul(a->den, b->den);

    UnlimitedRational* result = new UnlimitedRational(top, bot);
    delete ad; delete cb; delete top; delete bot;
    return result;
}

// ─── SUBTRACT ────────────────────────────────────────────────────────────────
// a/b - c/d = (a*d - c*b) / (b*d)

UnlimitedRational* UnlimitedRational::sub(UnlimitedRational* a, UnlimitedRational* b) {
    UnlimitedInt* ad  = UnlimitedInt::mul(a->num, b->den);
    UnlimitedInt* cb  = UnlimitedInt::mul(b->num, a->den);
    UnlimitedInt* top = UnlimitedInt::sub(ad, cb);
    UnlimitedInt* bot = UnlimitedInt::mul(a->den, b->den);

    UnlimitedRational* result = new UnlimitedRational(top, bot);
    delete ad; delete cb; delete top; delete bot;
    return result;
}

// ─── MULTIPLY ────────────────────────────────────────────────────────────────
// a/b * c/d = (a*c) / (b*d)

UnlimitedRational* UnlimitedRational::mul(UnlimitedRational* a, UnlimitedRational* b) {
    UnlimitedInt* top = UnlimitedInt::mul(a->num, b->num);
    UnlimitedInt* bot = UnlimitedInt::mul(a->den, b->den);

    UnlimitedRational* result = new UnlimitedRational(top, bot);
    delete top; delete bot;
    return result;
}

// ─── DIVIDE ──────────────────────────────────────────────────────────────────
// a/b ÷ c/d = (a*d) / (b*c)

UnlimitedRational* UnlimitedRational::div(UnlimitedRational* a, UnlimitedRational* b) {
    UnlimitedInt* top = UnlimitedInt::mul(a->num, b->den);
    UnlimitedInt* bot = UnlimitedInt::mul(a->den, b->num);

    UnlimitedRational* result = new UnlimitedRational(top, bot);
    delete top; delete bot;
    return result;
}