/*
 * src/type/array.h
 * Wopslang Array Template Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_ARRAY_H
#define WOPS_ARRAY_H
#include "variable.h"
#include "../error/signal.h"
#include <vector>
#include <cstdarg>

// class Array
// Temporary class used to store multiple variables
class Array {
    private:
    public:
    std::vector<Variable> container;

    Array() {
        container.clear();
    }

    Array(Variable o) {
        container.push_back(o);
    }

    Array(std::vector<Variable> o) {
        container.reserve(o.size());
        container = o;
    }
};

// struct ArrayWithCode
// Has Array class and Err enum as elements.
struct ArrayWithCode {
    Array var;
    Err error;
};

#endif
