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

// ─── GETTERS ─────────────────────────────────────────────────────────────────

UnlimitedInt* UnlimitedRational::get_num() const { return num; }
UnlimitedInt* UnlimitedRational::get_den() const { return den; }

// ─── SIMPLIFY ────────────────────────────────────────────────────────────────
// Divides numerator and denominator by their GCD
// e.g. 6/4 → gcd(6,4)=2 → 3/2

void UnlimitedRational::simplify() {
    if (num->get_sign() == 0) {
        // 0/anything = 0/1
        delete den;
        den = new UnlimitedInt(1);
        return;
    }

    UnlimitedInt* g = UnlimitedInt::gcd(num, den);

    UnlimitedInt* newNum = UnlimitedInt::div(num, g);
    UnlimitedInt* newDen = UnlimitedInt::div(den, g);

    // Fix sign: keep sign on numerator, denominator always positive
    newNum->get_sign();  // sign already correct from div
    newDen->get_sign();

    delete num;  delete den;  delete g;
    num = newNum;
    den = newDen;

    // Denominator should always be positive
    if (den->get_sign() == -1) {
        // flip both signs
        // num sign flip:
        UnlimitedInt* negNum = UnlimitedInt::mul(num, new UnlimitedInt(-1));
        UnlimitedInt* negDen = UnlimitedInt::mul(den, new UnlimitedInt(-1));
        delete num; delete den;
        num = negNum; den = negDen;
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