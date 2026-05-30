// src/unlimitedrational.cpp

#include "unlimitedrational.h"
#include <string>

// ─── CONSTRUCTORS ────────────────────────────────────────────────────────────

UnlimitedRational::UnlimitedRational(UnlimitedInt* numerator, UnlimitedInt* denominator) {
    // Deep copy both — we own our own pointers
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
    // No simplify needed — already simplified
}

UnlimitedRational& UnlimitedRational::operator=(const UnlimitedRational& other) {
    if (this == &other) return *this;
    delete num;
    delete den;
    num = new UnlimitedInt(*other.num);
    den = new UnlimitedInt(*other.den);
    return *this;
}

UnlimitedRational::~UnlimitedRational() {
    delete num;
    delete den;
}

// ─── GETTERS ─────────────────────────────────────────────────────────────────

UnlimitedInt* UnlimitedRational::get_num() const { return num; }
UnlimitedInt* UnlimitedRational::get_den() const { return den; }

// ─── SIMPLIFY ────────────────────────────────────────────────────────────────
  void UnlimitedRational::simplify() {
    // Case 1: numerator is zero → 0/1
    if (num->get_sign() == 0) {
        delete den;
        den = new UnlimitedInt(1);
        return;
    }

    // Case 2: denominator is already 1 → nothing to do
    if (den->to_string() == "1") return;

    // Case 3: use our own UnlimitedInt GCD — works for any size number
    // Make positive copies for GCD computation
    UnlimitedInt posNum(*num);
    posNum.set_sign(1);

    UnlimitedInt posDen(*den);
    posDen.set_sign(1);

    // Compute GCD using our own implementation
    UnlimitedInt* g = UnlimitedInt::gcd(&posNum, &posDen);

    // Only divide if GCD > 1
    if (g->to_string() != "1") {
        UnlimitedInt* newNum = UnlimitedInt::div(num, g);
        UnlimitedInt* newDen = UnlimitedInt::div(den, g);
        delete num;
        delete den;
        num = newNum;
        den = newDen;
    }
    delete g;

    // Denominator must always be positive
    // If negative, flip signs of both num and den
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

// ─── ARITHMETIC ──────────────────────────────────────────────────────────────

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

// a/b * c/d = (a*c) / (b*d)
UnlimitedRational* UnlimitedRational::mul(UnlimitedRational* a, UnlimitedRational* b) {
    UnlimitedInt* top = UnlimitedInt::mul(a->num, b->num);
    UnlimitedInt* bot = UnlimitedInt::mul(a->den, b->den);
    UnlimitedRational* result = new UnlimitedRational(top, bot);
    delete top; delete bot;
    return result;
}

// a/b / c/d = (a*d) / (b*c)
UnlimitedRational* UnlimitedRational::div(UnlimitedRational* a, UnlimitedRational* b) {
    UnlimitedInt* top = UnlimitedInt::mul(a->num, b->den);
    UnlimitedInt* bot = UnlimitedInt::mul(a->den, b->num);
    UnlimitedRational* result = new UnlimitedRational(top, bot);
    delete top; delete bot;
    return result;
}