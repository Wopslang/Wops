/*
 * src/error/signal.h
 * Wopslang Error Signal Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_ERRORSIGNAL_H
#define WOPS_ERRORSIGNAL_H

#include <iostream>

// enum Err
// Enumeration of error types
enum Err {
    OK,
    ERROR,
    INTERPRETER_CANNOT_OPEN_FILE,
    NO_MATCHING_SYNTAX_FOR,
    NO_MATCHING_SYNTAX_IF,
    VARIABLE_HAS_NOT_DECLARED,
    VARIABLE_REDECLARE,
    ERROR_OCCURED_WHILE_CALLING_FUNCTION,
    BREAK_CONTINUE_ONLY_ALLOWED_FOR,
    ASSIGN_ON_CONSTANT,
    ASSIGN_ON_UNKNOWN,
    IF_NO_BOOLEAN_CONDITION,
    ELIF_NO_BOOLEAN_CONDITION,
    FOR_NO_BOOLEAN_CONDITION,
};

class ErrHandler {
    public:
    void CallErr(int error_pos, Err errtype, std::vector<String> arg={}) {
        std::string errmsg;
        switch (errtype) {
            case INTERPRETER_CANNOT_OPEN_FILE:
            errmsg = "From Interpreter: cannot open the file";
            break;
            case NO_MATCHING_SYNTAX_FOR:
            errmsg = "No matching syntax: for";
            break;
            case NO_MATCHING_SYNTAX_IF:
            errmsg = "No matching syntax: if";
            break;
            case VARIABLE_HAS_NOT_DECLARED:
            errmsg = arg[0] + " has not declared yet";
            break;
            case VARIABLE_REDECLARE:
            errmsg = "Redeclare variable " + arg[0];
            break;
            case ERROR_OCCURED_WHILE_CALLING_FUNCTION:
            errmsg = "Error occured while calling " + arg[0];
            break;
            case BREAK_CONTINUE_ONLY_ALLOWED_FOR:
            errmsg = "break and continue statement only allowed to be used in for statements";
            break;
            case ASSIGN_ON_CONSTANT:
            errmsg = arg[0] + " is constant";
            break;
            case ASSIGN_ON_UNKNOWN:
            errmsg = "Variable " + arg[0] + " hasn't declared yet";
            break;
            case IF_NO_BOOLEAN_CONDITION:
            errmsg = "If Statement allows only boolean condition expression.";
            break;
            case ELIF_NO_BOOLEAN_CONDITION:
            errmsg = "Elif Statement allows only boolean condition expression.";
            break;
            case FOR_NO_BOOLEAN_CONDITION:
            errmsg = "For Statement allows only boolean condition expression.";
            break;
        }
        if (error_pos != -1) {
            std::cout << "\e[31m"<< "line " << error_pos << ": " << errmsg << "\e[m";
            exit(1);
        }
        std::cout << "\e[31m"<< "runtime: " << errmsg << "\e[m";
        exit(1);
        return;
    }
};

#endif
