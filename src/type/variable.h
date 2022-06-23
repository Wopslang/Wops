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
    int runtime_codeline; // for error analyzing
    TYPE _t;

    // constructor
    Variable(std::string varname = "_", std::string val = "", TYPE t = OPERATOR, int _runtime_codeline = -1, bool con = 0) {
        runtime_codeline = _runtime_codeline;
        _t = t;
        token = varname;
        constant = con;

        if (varname == "") ErrHandler().CallErrDE(runtime_codeline, "Name of variable should not be blank. How about using '_'?");
        if (Substitute(val) == ERROR) ErrHandler().CallErrDE(runtime_codeline, "Type of value does not match with declaration");
    }

    // operation
    Variable operator + (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        switch (_t) {
            case BOOL:
            case INT:
                if (operand._t == STRING) ErrHandler().CallErrDE(runtime_codeline, "No matching operation between int and string was found.");
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string((Int)(std::stoi(value)+std::stod(operand.value))));
                else
                    res.Substitute(std::to_string(std::stoi(value)+std::stoi(operand.value)));
                break;
            case DOUBLE:
                if (operand._t == STRING) ErrHandler().CallErrDE(runtime_codeline, "No matching operation between double and string was found.");
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string(std::stod(value)+std::stod(operand.value)));
                else
                    res.Substitute(std::to_string(std::stod(value)+std::stoi(operand.value)));
                break;
            case STRING:
                if (operand._t != STRING) ErrHandler().CallErrDE(runtime_codeline, "The only string can operate with string.");
                res.Substitute("\""+trim(value)+trim(operand.value)+"\"");
                break;
        }
        return res;
    }
    
    Variable operator - (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErrDE(runtime_codeline, "No matching operation with string and any was found.");
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
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErrDE(runtime_codeline, "No matching operation with string and any was found.");
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
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErrDE(runtime_codeline, "No matching operation with string and any was found.");
        if (operand.GetValue() == "0") ErrHandler().CallErrDE(runtime_codeline, "Not allowed to divide with zero");
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
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErrDE(runtime_codeline, "No matching operation with string and any was found.");
        if (operand.GetValue() == "0") ErrHandler().CallErrDE(runtime_codeline, "Not allowed to divide with zero");
        switch (_t) {
            case INT:
                res.Substitute(std::to_string(std::stoi(value)%std::stoi(operand.value)));
                break;
        }
        return res;
    }

    Variable operator == (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        if (operand._t != _t) ErrHandler().CallErrDE(runtime_codeline, "Not allowed to compare between different type variables.");
        res.Substitute(std::to_string(operand.GetValue()==GetValue()));

        return res;
    }

    Variable operator != (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        if (operand._t != _t) ErrHandler().CallErrDE(runtime_codeline, "Not allowed to compare between different type variables.");
        res.Substitute(std::to_string(operand.GetValue()!=GetValue()));

        return res;
    }

    Variable operator > (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()>GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErrDE(runtime_codeline, "Something went wrong :(");
        }

        return res;
    }

    Variable operator < (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()<GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErrDE(runtime_codeline, "Something went wrong :(");
        }

        return res;
    }

    Variable operator >= (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()>=GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErrDE(runtime_codeline, "Something went wrong :(");
        }

        return res;
    }

    Variable operator <= (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()<=GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErrDE(runtime_codeline, "Something went wrong :(");
        }

        return res;
    }

    Variable operator ! () {
        Variable res = Variable("_", "", BOOL);

        if (_t != BOOL) ErrHandler().CallErrDE(runtime_codeline, "Operation ! allows only boolean variables.");
        res.Substitute(std::to_string(
            !(std::stoi(GetValue()))
        ));
        return res;
    }

    Variable operator && (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);
        if (_t != BOOL || operand._t != BOOL) ErrHandler().CallErrDE(runtime_codeline, "Operation && allows only boolean variables.");

        res.Substitute(std::to_string(std::stoi(operand.GetValue())&&std::stoi(GetValue())));

        return res;
    }

    Variable operator || (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErrDE(runtime_codeline, "Operand should not be blank.");
        Variable res = Variable("_", "", BOOL);
        if (_t != BOOL || operand._t != BOOL) ErrHandler().CallErrDE(runtime_codeline, "Operation || allows only boolean variables.");

        res.Substitute(std::to_string(std::stoi(operand.GetValue())||std::stoi(GetValue())));

        return res;
    }

    // management
    inline Err Substitute(std::string newval); // substitute
    inline std::string GetValue(); // extract

    // utility
    inline std::string trim(std::string s) { s = s.erase(0,1); return s.erase(s.length()-1, 1); }
};

inline Err Variable::Substitute(std::string newval) {
    if (newval == "") return OK;
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
