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

// enum TYPE {INT, DOUBLE, STRING, BOOL, OPERATOR}
// Enumeration of data types
enum TYPE {
    INT,
    DOUBLE,
    STRING,
    BOOL,
    OPERATOR // +, -, *, ...
};

const std::string type_strname[] = {"INT", "DOUBLE", "STRING", "BOOL", "OPERATOR"};
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
    String value;
    String token;
    bool constant = 0;
    int runtime_codeline; // for error analyzing
    TYPE _t;

    // constructor
    Variable(String varname = "_", String val = "", TYPE t = OPERATOR, int _runtime_codeline = -1, bool con = 0) {
        runtime_codeline = _runtime_codeline;
        _t = t;
        token = varname;
        constant = con;

        if (varname == "") ErrHandler().CallErr(runtime_codeline, BLANK_VARIABLE_NAME, {});
        if (Substitute(val) == ERROR) ErrHandler().CallErr(runtime_codeline, NOT_MATCHING_TYPE_WITH_DEF, {varname, type_strname[t]});
    }

    // operation
    Variable operator + (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", _t);

        switch (_t) {
            case BOOL:
            case INT:
                if (operand._t == STRING) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE, {"+", "int", "string"});
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string((Int)(std::stoi(value)+std::stod(operand.value))));
                else
                    res.Substitute(std::to_string(std::stoi(value)+std::stoi(operand.value)));
                break;
            case DOUBLE:
                if (operand._t == STRING) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE, {"+", "double", "string"});
                if (operand._t == DOUBLE)
                    res.Substitute(std::to_string(std::stod(value)+std::stod(operand.value)));
                else
                    res.Substitute(std::to_string(std::stod(value)+std::stoi(operand.value)));
                break;
            case STRING:
                if (operand._t != STRING) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE, {"+", "string", type_strname[operand._t]});
                res.Substitute("\""+trim(value)+trim(operand.value)+"\"");
                break;
        }
        return res;
    }
    
    Variable operator - (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE, {"-", type_strname[_t], type_strname[operand._t]});
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
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE, {"*", type_strname[_t], type_strname[operand._t]});
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
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE, {"/", type_strname[_t], type_strname[operand._t]});
        if (operand.GetValue() == "0") ErrHandler().CallErr(runtime_codeline, DIVIDING_WITH_ZERO, {});
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
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", _t);

        if (operand._t == STRING || _t == STRING) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE, {"%", type_strname[_t], type_strname[operand._t]});
        if (operand.GetValue() == "0") ErrHandler().CallErr(runtime_codeline, DIVIDING_WITH_ZERO, {});
        switch (_t) {
            case INT:
                res.Substitute(std::to_string(std::stoi(value)%std::stoi(operand.value)));
                break;
        }
        return res;
    }

    Variable operator == (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", BOOL);

        if (operand._t != _t) ErrHandler().CallErr(runtime_codeline, NOT_MATCHING_TYPE_WHEN_COMP, {type_strname[_t], type_strname[operand._t]});
        res.Substitute(std::to_string(operand.GetValue()==GetValue()));

        return res;
    }

    Variable operator != (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", BOOL);

        if (operand._t != _t) ErrHandler().CallErr(runtime_codeline, NOT_MATCHING_TYPE_WHEN_COMP, {type_strname[_t], type_strname[operand._t]});
        res.Substitute(std::to_string(operand.GetValue()!=GetValue()));

        return res;
    }

    Variable operator > (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()>GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErr(runtime_codeline, ERROR_OCCURED_WHILE_CALLING_FUNCTION, {">"});
        }

        return res;
    }

    Variable operator < (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()<GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErr(runtime_codeline, ERROR_OCCURED_WHILE_CALLING_FUNCTION, {"<"});
        }

        return res;
    }

    Variable operator >= (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()>=GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErr(runtime_codeline, ERROR_OCCURED_WHILE_CALLING_FUNCTION, {">="});
        }

        return res;
    }

    Variable operator <= (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", BOOL);

        try {
            res.Substitute(std::to_string(operand.GetValue()<=GetValue()));
        }
        catch(const std::exception& e) {
            ErrHandler().CallErr(runtime_codeline, ERROR_OCCURED_WHILE_CALLING_FUNCTION, {"<="});
        }

        return res;
    }

    Variable operator ! () {
        Variable res = Variable("_", "", BOOL);

        if (_t != BOOL) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE_UNARY, {"!"});
        res.Substitute(std::to_string(
            !(std::stoi(GetValue()))
        ));
        return res;
    }

    Variable operator && (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", BOOL);
        if (_t != BOOL || operand._t != BOOL) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE, {"&&", type_strname[_t], type_strname[operand._t]});

        res.Substitute(std::to_string(std::stoi(operand.GetValue())&&std::stoi(GetValue())));

        return res;
    }

    Variable operator || (Variable operand) {
        if (operand.GetValue() == "" || GetValue() == "") ErrHandler().CallErr(runtime_codeline, BLANK_OPERAND, {});
        Variable res = Variable("_", "", BOOL);
        if (_t != BOOL || operand._t != BOOL) ErrHandler().CallErr(runtime_codeline, NO_OPERATION_MATCHING_TYPE, {"||", type_strname[_t], type_strname[operand._t]});

        res.Substitute(std::to_string(std::stoi(operand.GetValue())||std::stoi(GetValue())));

        return res;
    }

    // management
    inline Err Substitute(String newval); // substitute
    inline String GetValue(); // extract

    // utility
    inline String trim(String s) { s = s.erase(0,1); return s.erase(s.length()-1, 1); }
};

inline Err Variable::Substitute(String newval) {
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

inline String Variable::GetValue() { return value; }

// struct VariableWithCode
// Has Variable class and Err enum as elements.
struct VariableWithCode {
    Variable var;
    Err error;
};

#endif
