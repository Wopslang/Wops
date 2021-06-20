/*
 * src/type/variable.h
 * Wopslang Variable Template Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_VARIABLE_H
#define WOPS_VARIABLE_H
#include <string>
#include "../error/signal.h"

using Int=signed int;
using Double=double;
using String=std::string;
using Bool=bool;

// enum TYPE {INT, DOUBLE, STRING, BOOL}
// Enumeration of data types
enum TYPE {
    INT,
    DOUBLE,
    STRING,
    BOOL,
};

/**
 * class Variable
 * Variable container class: each variable has a single value.
 *
 * Structure
 *
 * Variable
 *   - value
 *   - token (=variable name)
 *   - type
*/
class Variable {
    private:
    public:
    Variable(std::string varname, std::string val, TYPE t) { // constructor
        assert(varname != "");
        value = val;
        token = varname;
        _t = t;
    }

    Err Substitute(std::string newval) { // substitute
        // :TODO add type checker
        value = newval;
        return OK;
    }

    std::string GetValue() { // extract value
        return value;
    }

    std::string value;
    std::string token;
    TYPE _t;
};

// struct VariableWithCode
// Has Variable class and Err enum as elements.
struct VariableWithCode {
    Variable var;
    Err error;
};

#endif
