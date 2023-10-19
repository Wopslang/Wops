/*
 * src/error/signal.h
 * Wopslang Error Signal Headerfile
 *
 * 2023, Wops Team
 * */

#ifndef WOPS_ERRORSIGNAL_H
#define WOPS_ERRORSIGNAL_H

#include <iostream>
#include <vector>

#include "../../include/basis.h"

// enum Err
// Enumeration of error types
enum Err {
  OK,
  ERROR,
  INTERPRETER_CANNOT_OPEN_FILE,
  BLANK_VARIABLE_NAME,
  BLANK_OPERAND,
  BLANK_PARAMETER,
  NO_IDENTIFIER_AFTER_OPERATOR,
  NO_OPERATION_MATCHING_TYPE,
  NO_OPERATION_MATCHING_TYPE_UNARY,
  NO_MATCHING_UNARY_OPERATION_FORM,
  NO_MATCHING_SYNTAX_EXPRESSION,
  NO_MATCHING_SYNTAX_FOR,
  NO_MATCHING_SYNTAX_IF,
  NO_MATCHING_SYNTAX_ELIF,
  NO_MATCHING_STRUCTURE_IF,
  NO_MATCHING_SYNTAX_BREAK,
  NO_MATCHING_SYNTAX_CONTINUE,
  NOT_MATCHING_TYPE_WITH_DEF,
  NOT_MATCHING_TYPE_WHEN_COMP,
  NOT_CLOSED_BLOCK,
  VARIABLE_HAS_NOT_DECLARED,
  VARIABLE_REDECLARE,
  ERROR_OCCURED_WHILE_CALLING_FUNCTION,
  BREAK_CONTINUE_ONLY_ALLOWED_FOR,
  ASSIGN_ON_CONSTANT,
  ASSIGN_ON_UNKNOWN,
  IF_NO_BOOLEAN_CONDITION,
  ELIF_NO_BOOLEAN_CONDITION,
  FOR_NO_BOOLEAN_CONDITION,
  CANNOT_LOAD_LIBRARY,
  CANNOT_LOAD_SYMBOL,
  OBJECT_OVERFLOW,
  OBJECT_NOT_MATCHING_DATA,
  OBJECT_WRONG_DIMENSION,
  TOO_HIGH_DIMENSION,
  UNMATCHED_PARENTHESIS,
  OPERATION_ONLY_BINARY,
  DIVIDING_WITH_ZERO,
};

class ErrHandler {
 public:
  void CallErr(int error_pos, Err errtype, std::vector<String> arg) {
    std::string errmsg;
    switch (errtype) {
      case OK:
        errmsg = "OKAY";
        break;
      case ERROR:
        errmsg = "ERROR";
        break;
      case INTERPRETER_CANNOT_OPEN_FILE:
        errmsg = "From Interpreter: cannot open the file";
        break;
      case BLANK_VARIABLE_NAME:
        errmsg = "Name of variable should not be blank";
        break;
      case BLANK_OPERAND:
        errmsg = "Operand should not be blank";
        break;
      case BLANK_PARAMETER:
        errmsg = "Parameter should not be blank";
        break;
      case NO_IDENTIFIER_AFTER_OPERATOR:
        errmsg = "Operator " + arg[0] + " cannot appear after identifer";
        break;
      case NO_OPERATION_MATCHING_TYPE:
        errmsg = "Operator " + arg[0] + " doesn't have operation between " + arg[1] + " and " + arg[2];
        break;
      case NO_OPERATION_MATCHING_TYPE_UNARY:
        errmsg = "Operator " + arg[0] + " doesn't have operation with " + arg[1];
        break;
      case NO_MATCHING_UNARY_OPERATION_FORM:
        errmsg = "No matching unary operation form: " + arg[0];
        break;
      case NO_MATCHING_SYNTAX_EXPRESSION:
        errmsg = "No matching syntax: expression";
        break;
      case NO_MATCHING_SYNTAX_FOR:
        errmsg = "No matching syntax: for";
        break;
      case NO_MATCHING_SYNTAX_IF:
        errmsg = "No matching syntax: if";
        break;
      case NO_MATCHING_SYNTAX_ELIF:
        errmsg = "No matching syntax: elif";
        break;
      case NO_MATCHING_STRUCTURE_IF:
        errmsg = "No matching structure: if-elif-else";
        break;
      case NO_MATCHING_SYNTAX_BREAK:
        errmsg = "No matching syntax: break";
        break;
      case NO_MATCHING_SYNTAX_CONTINUE:
        errmsg = "No matching syntax: continue";
        break;
      case NOT_MATCHING_TYPE_WITH_DEF:
        errmsg = "Type of " + arg[0] + " doesn't match with definition, " + arg[1];
        break;
      case NOT_MATCHING_TYPE_WHEN_COMP:
        errmsg = "Comparing with different type variables is not allowed (" + arg[0] + ", " + arg[1] + ")";
        break;
      case NOT_CLOSED_BLOCK:
        errmsg = "Block is not closed";
        break;
      case VARIABLE_HAS_NOT_DECLARED:
        errmsg = arg[0] + " has not declared yet";
        break;
      case VARIABLE_REDECLARE:
        errmsg = "Redeclared variable " + arg[0] + "";
        break;
      case ERROR_OCCURED_WHILE_CALLING_FUNCTION:
        errmsg = "Error occured while calling " + arg[0] + "";
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
        errmsg = "If Statement allows only boolean condition expression";
        break;
      case ELIF_NO_BOOLEAN_CONDITION:
        errmsg = "Elif Statement allows only boolean condition expression";
        break;
      case FOR_NO_BOOLEAN_CONDITION:
        errmsg = "For Statement allows only boolean condition expression";
        break;
      case CANNOT_LOAD_LIBRARY:
        errmsg = "Cannot load library: " + arg[0] + "";
        break;
      case CANNOT_LOAD_SYMBOL:
        errmsg = "Cannot load symbol: " + arg[0] + "";
        break;
      case OBJECT_OVERFLOW:
        errmsg = "Size of object " + arg[0] + " is over than limit";
        break;
      case OBJECT_NOT_MATCHING_DATA:
        errmsg = "Data of object " + arg[0] + " does not match with declaration";
        break;
      case OBJECT_WRONG_DIMENSION:
        errmsg = "Object " + arg[0] + " has wrong dimension (Expected " + arg[1] + ", Found " + arg[2] + ")";
        break;
      case TOO_HIGH_DIMENSION:
        errmsg = arg[0] + " has too high dimension for job (Expected " + arg[1] + ", Found " + arg[2] + ")";
        break;
      case UNMATCHED_PARENTHESIS:
        errmsg = "Unmatched parenthesis";
        break;
      case OPERATION_ONLY_BINARY:
        errmsg = "Operation " + arg[0] + " cannot be unary";
        break;
      case DIVIDING_WITH_ZERO:
        errmsg = "Dividing with zero is not allowed";
        break;
    }
    if (error_pos != -1) {
      std::cout << "\e[31m"
                << "line " << error_pos << ": " << errmsg << "\e[m.";
      exit(1);
    }
    std::cout << "\e[31m"
              << "runtime: " << errmsg << "\e[m.";
    exit(1);
    return;
  }
};

#endif