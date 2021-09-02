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

// enum TYPE {INT, DOUBLE, STRING, BOOL, OPERATOR}
// Enumeration of data types
enum TYPE {
    INT,
    DOUBLE,
    STRING,
    BOOL,
    OPERATOR // +, -, *, ...
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
    std::string value;
    std::string token;
    bool constant = 0;
    TYPE _t;

    // constructor
    Variable(std::string varname = "_", std::string val = "", TYPE t = INT, bool con = 0) {
        if (varname == "") ErrHandler().CallErr("Name of variable should not be blank. How about using '_'?");

        _t = t;
        token = varname;
        constant = con;
        Substitute(val);
    }

    // operation
    Variable operator + (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        switch (_t) {
            case BOOL:
            case INT:
                assert(operand._t != STRING && "No matching operation between int and string was found.");
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string((Int)(std::stoi(value)+std::stod(operand.value))));
                else
                    res.Substitute(std::to_string(std::stoi(value)+std::stoi(operand.value)));
                break;
            case DOUBLE:
                assert(operand._t != STRING && "No matching operation between double and string was found.");
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string(std::stod(value)+std::stod(operand.value)));
                else
                    res.Substitute(std::to_string(std::stod(value)+std::stoi(operand.value)));
                break;
            case STRING:
                assert(operand._t == STRING && "The only string can operate with string.");
                res.Substitute("\""+trim(value)+trim(operand.value)+"\"");
                break;
        }
        return res;
    }
    
    Variable operator - (Variable operand) {
        assert(operand.GetValue() != "" && GetValue() != "" && "Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        assert(operand._t != STRING && _t != STRING && "No matching operation with string and any was found.");
        switch (_t) {
            case BOOL:
            case INT:
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string((Int)(std::stoi(value)-std::stod(operand.value))));
                else
                    res.Substitute(std::to_string(std::stoi(value)-std::stoi(operand.value)));
                break;
            case DOUBLE:
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string(std::stod(value)-std::stod(operand.value)));
                else
                    res.Substitute(std::to_string(std::stod(value)-std::stoi(operand.value)));
                break;
        }
        return res;
    }

    Variable operator * (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErr("No matching operation with string and any was found.");
        switch (_t) {
            case BOOL:
            case INT:
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string((Int)(std::stoi(value)*std::stod(operand.value))));
                else
                    res.Substitute(std::to_string(std::stoi(value)*std::stoi(operand.value)));
                break;
            case DOUBLE:
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string(std::stod(value)*std::stod(operand.value)));
                else
                    res.Substitute(std::to_string(std::stod(value)*std::stoi(operand.value)));
                break;
        }
        return res;
    }

    Variable operator / (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErr("No matching operation with string and any was found.");
        if (operand.GetValue() == "0") ErrHandler().CallErr("Not allowed to divide with zero");
        switch (_t) {
            case BOOL:
            case INT:
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string((Int)(std::stoi(value)/std::stod(operand.value))));
                else
                    res.Substitute(std::to_string(std::stoi(value)/std::stoi(operand.value)));
                break;
            case DOUBLE:
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string(std::stod(value)/std::stod(operand.value)));
                else
                    res.Substitute(std::to_string(std::stod(value)/std::stoi(operand.value)));
                break;
        }
        return res;
    }

    Variable operator % (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErr("No matching operation with string and any was found.");
        if (operand.GetValue() == "0") ErrHandler().CallErr("Not allowed to divide with zero");
        switch (_t) {
            case INT:
                res.Substitute(std::to_string(std::stoi(value)%std::stoi(operand.value)));
                break;
        }
        return res;
    }

    Variable operator == (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        if (operand._t != _t) ErrHandler().CallErr("Not allowed to compare between different type variables.");
        res.Substitute(std::to_string(operand.GetValue()==GetValue()));

        return res;
    }

    Variable operator != (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        if (operand._t != _t) ErrHandler().CallErr("Not allowed to compare between different type variables.");
        res.Substitute(std::to_string(operand.GetValue()!=GetValue()));

        return res;
    }

    Variable operator > (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()>GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErr("Something went wrong :(");
        }

        return res;
    }

    Variable operator < (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()<GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErr("Something went wrong :(");
        }

        return res;
    }

    Variable operator >= (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()>=GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErr("Something went wrong :(");
        }

        return res;
    }

    Variable operator <= (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()<=GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErr("Something went wrong :(");
        }

        return res;
    }

    Variable operator ! () {
        Variable res = Variable("_", "", BOOL);

        if (_t != BOOL) ErrHandler().CallErr("Operation ! allows only boolean variables.");
        res.Substitute(std::to_string(
            !(std::stoi(GetValue()))
        ));
        return res;
    }

    Variable operator && (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);
        if (_t != BOOL || operand._t != BOOL) ErrHandler().CallErr("Operation && allows only boolean variables.");

        res.Substitute(std::to_string(std::stoi(operand.GetValue())&&std::stoi(GetValue())));

        return res;
    }

    Variable operator || (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr("Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);
        if (_t != BOOL || operand._t != BOOL) ErrHandler().CallErr("Operation || allows only boolean variables.");

        res.Substitute(std::to_string(std::stoi(operand.GetValue())||std::stoi(GetValue())));

        return res;
    }

    // management
    inline Err Substitute(std::string newval); // substitute
    inline std::string GetValue(); // extract

    // utility
    inline std::string trim(std::string s) { return s.erase(0,1).erase(s.length()-1, 1); }
};

inline Err Variable::Substitute(std::string newval) {
    try {
        switch (_t) {
            case INT:
                value = std::to_string(std::stoi(newval));
                break;
            case DOUBLE:
                value = std::to_string(std::stod(newval));
                break;
            case BOOL:
                value = std::to_string(std::stoi(newval));
                break;
            case STRING:
                if (newval == "") {value = newval; break;}
                if (newval[0] != '"' || newval[newval.length()-1] != '"' || newval.length() < 2) return ERROR;
                value = newval;
                break;
            case OPERATOR:
                value = newval;
                break;
        }
    }
    catch(const std::exception& e) {
        return ERROR;
    }

    return OK;
}

inline std::string Variable::GetValue() { return value; }

// struct VariableWithCode
// Has Variable class and Err enum as elements.
struct VariableWithCode {
    Variable var;
    Err error;
};

#endif
