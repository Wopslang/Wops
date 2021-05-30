/*
 * src/type/variable.h
 * Wopslang Variable Template Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_VARIABLE_H
#define WOPS_VARAIBLE_H
#include <string>
#include "src/error/signal.h"

enum TYPE {
    INT,
    DOUBLE,
    STRING,
    BOOL,
};

class Variable {
    private:
    public:
    Variable(std::string varname, std::string val, TYPE t) {
        assert(varname != "");
        value = val;
        token = varname;
        _t = t;
    }

    Err Substitute(std::string newval) {
        // :TODO add type checker
        value = newval;
        return OK;
    }

    std::string value;
    std::string token;
    TYPE _t;
};

#endif