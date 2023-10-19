/*
 * src/runtime/AST.h
 * Wopslang AST Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_AST_H
#define WOPS_AST_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../import_/eexec_.h"
#include "../type/object.h"
#include "../type/operator.h"
#include "../type/variable.h"

// enum TYPE {...}
// Enumeration of statement types
enum StmtType {
  Main,
  ConstDel,
  VarDel,
  Expression,
  Assignment,
  BreakStmt,
  ContinueStmt,
  IfStmt,
  ElifStmt,
  ElseStmt,
  ForStmt,
  BracketBlock,  // ()
};

typedef std::unordered_map<std::string, Object> Storage;

/**
 * class Expr
 * Expression Syntax Tree class
 *
 * Structure
 *
 * private
 *   - bool constant, variable, call (token's type)
 *   - Variable token
 *   - std::vector<Expr> children
 *   - int codeline
 */

class Expr {
 private:
  bool constant = 0, variable = 0, call = 0;
  Object token;  // base object (variable)
  std::vector<Expr> children;
  int codeline;

 public:
  // constructor
  Expr(std::vector<bool> t, Object tkn, int _codeline) { token = tkn, constant = t[0], variable = t[1], call = t[2], codeline = _codeline; }

  void SetChildren(std::vector<Expr> rplcment) { children = rplcment; }

  void AddChild(Expr child) { children.push_back(child); }

  // execute from the tree root
  Object Execute(std::vector<Storage>& storages) {
    std::string tkn = token.GetBase().GetValue();

    if (variable) {
      auto iter = storages[0].find(tkn);
      bool wasErrorOccured = true;
      Object ret;
      for (Storage storage : storages) {
        iter = storage.find(tkn);
        if (iter != storage.end()) {
          ret = iter->second;
          wasErrorOccured = false;
          break;
        }
      }
      if (wasErrorOccured)  // has variable declared?
        ErrHandler().CallErr(codeline, VARIABLE_HAS_NOT_DECLARED, {tkn});

      ret.runtime_codeline = codeline;
      return ret;
    }
    if (constant) {
      return Object("_", {}, {}, token.GetBase(), 0, codeline, OK);
    }
    if (call) {
      Object arg;
      std::vector<Object> container;
      for (Expr child : children) {
        container.push_back(child.Execute(storages));
      }
      arg.runtime_codeline = codeline;
      arg.dim = 1;
      arg.ReplaceContainer(container);
      arg.size = {(Int)container.size()};
      Object res = EExecFunc(tkn, arg);
      if (res.errvalue != OK) ErrHandler().CallErr(codeline, res.errvalue, {tkn});

      return res;
    }

    // Binary Operation
    if (tkn == "+") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() + operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "-") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() - operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "*") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() * operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "/") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() / operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "%") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() % operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "==") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() == operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "!=") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() != operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == ">") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() > operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == ">=") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() >= operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "<") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() < operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "<=") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() <= operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "||") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() || operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }
    if (tkn == "&&") {
      Object value = children[0].Execute(storages), operand = children[1].Execute(storages);
      if (value.dim != operand.dim) ErrHandler().CallErr(codeline, OBJECT_WRONG_DIMENSION, {tkn, std::to_string(value.dim), std::to_string(operand.dim)});
      if (value.dim == 0) return Object("_", {}, {}, value.GetBase() && operand.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(operand.dim)});
    }

    // Unary Operation
    if (tkn == "!") {
      Object value = children[0].Execute(storages);
      if (value.dim == 0) return Object("_", {}, {}, !value.GetBase(), 0, codeline, OK);
      ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {tkn, "0", std::to_string(value.dim)});
    }
  }
};

/**
 * class AST
 * Abstract Syntax Tree class
 *
 * Structure
 *
 * private
 *   - StmtType _t
 *	 - std::vector<AST *> childStmt
 *	 - std::vector<Expr> expression
 *	 - std::vector<Variable> argument
 * public
 *	 - int codeline
 *
 * expression
 *
 * if AST's type is one of these:
 *   - ConstDel
 *   - VarDel
 *   - Expression
 *   - Assignment
 *   - IfStmt
 *   - ElifStmt
 *   - ElseStmt
 *   - ForClauseStmt
 *   - ForSCStmt
 *
 * expression will not be blank. (length 1)
 *
 * argument
 *
 * argument can have different formation.
 *   - _t = ConstDel or VarDel: argument[2] = {TYPE, IDENTIFIER}
 *   - _t = Assignment: argument[1] = {IDENTIFIER}
 *   - _t = ForClauseStmt: argument[1] = {VARIABLE} (+ START, STOP, STEP expr)
 *   - others: argument[0] = {}
 *
 * every argument's type(Variable::_t) should be OPERATOR.
 */
class AST {
 private:
  StmtType _t;
  std::vector<AST> childStmt;
  std::vector<Expr> expression;
  std::vector<Object> argument;

 public:
  int codeline;

  // constructor
  AST(StmtType t, std::vector<Object> argv, std::vector<Expr> expr, int _codeline) { _t = t, argument = argv, expression = expr, codeline = _codeline; }

  void SetChildren(std::vector<AST> rplcment) { childStmt = rplcment; }

  void AddChild(AST child) { childStmt.push_back(child); }

  void SetExpr(std::vector<Expr> rplcment) { expression = rplcment; }

  void AddExpr(Expr expr) { expression.push_back(expr); }

  void SetArg(std::vector<Object> argv) { argument = argv; }

  void AddArg(Object argv) { argument.push_back(argv); }

  StmtType GetStmt() { return _t; }

  // storages: smaller index, inner variable
  std::pair<int, bool> Execute(std::vector<Storage>& storages) {
    switch (_t) {
      case Main: {
        bool ignoreif = 0;
        for (AST ast : childStmt) {
          if (ignoreif) {
            if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
            ignoreif = 0;
          }

          std::pair<int, bool> res = ast.Execute(storages);
          if (res.first >= 1) ErrHandler().CallErr(codeline, BREAK_CONTINUE_ONLY_ALLOWED_FOR, {});

          if (res.second) {
            ignoreif = 1;
            continue;
          }
        }
        break;
      }

      case ConstDel: {
        Object v_type = argument[0], v_identifier = argument[1];
        if (v_type.dim != 0 && v_identifier.dim != 0) ErrHandler().CallErr(codeline, TOO_HIGH_DIMENSION, {"Expression", "0", std::to_string(v_type.dim)});
        for (Storage& storage : storages) {
          if (storage.find(v_identifier.GetBase().GetValue()) != storage.end()) ErrHandler().CallErr(codeline, VARIABLE_REDECLARE, {v_identifier.GetBase().GetValue()});
        }

        TYPE v_t = v_type.GetBase().GetValue() == "int" ? INT : (v_type.GetBase().GetValue() == "bool" ? BOOL : (v_type.GetBase().GetValue() == "string" ? STRING : DOUBLE));

        storages[0].insert({v_identifier.GetBase().GetValue(), {v_identifier.GetBase().GetValue(), {}, {}, Variable(v_identifier.GetBase().GetValue(), expression[0].Execute(storages).GetBase().GetValue(), v_t, true), 0, codeline, OK}});

        break;
      }

      case VarDel: {
        Object v_type = argument[0], v_identifier = argument[1];
        for (Storage& storage : storages) {
          if (storage.find(v_identifier.GetBase().GetValue()) != storage.end()) ErrHandler().CallErr(codeline, VARIABLE_REDECLARE, {v_identifier.GetBase().GetValue()});
        }

        TYPE v_t = v_type.GetBase().GetValue() == "int" ? INT : (v_type.GetBase().GetValue() == "bool" ? BOOL : (v_type.GetBase().GetValue() == "string" ? STRING : DOUBLE));

        storages[0].insert({v_identifier.GetBase().GetValue(), {v_identifier.GetBase().GetValue(), {}, {}, Variable(v_identifier.GetBase().GetValue(), expression[0].Execute(storages).GetBase().GetValue(), v_t, false), 0, codeline, OK}});
        break;
      }

      case Expression:
        expression[0].Execute(storages);
        break;

      case Assignment: {
        Object v_identifier = argument[0];
        bool wasErrorOccured = true;
        for (Storage& storage : storages)
          if (storage.find(v_identifier.GetBase().GetValue()) != storage.end()) {
            if (storage[v_identifier.GetBase().GetValue()].GetBase().constant) ErrHandler().CallErr(codeline, ASSIGN_ON_CONSTANT, {v_identifier.GetBase().GetValue()});
            storage[v_identifier.GetBase().GetValue()].GetBase().Substitute(expression[0].Execute(storages).GetBase().GetValue());
            wasErrorOccured = false;
            break;
          }
        if (wasErrorOccured) ErrHandler().CallErr(codeline, ASSIGN_ON_UNKNOWN, {v_identifier.GetBase().GetValue()});
        break;
      }

      case BreakStmt:
        return {2, false};

      case ContinueStmt:
        return {1, false};

      case IfStmt: {
        storages.insert(storages.begin(), Storage());
        Object condition = expression[0].Execute(storages);
        if (condition.GetBase()._t != BOOL) ErrHandler().CallErr(codeline, IF_NO_BOOLEAN_CONDITION, {});
        if (condition.GetBase().GetValue() == "0") {
          return {0, false};
        }
        bool ignoreif = 0;
        for (AST ast : childStmt) {
          if (ignoreif) {
            if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
            ignoreif = 0;
          }
          std::pair<int, bool> res = ast.Execute(storages);
          if (res.first >= 1) {
            return {res.first, res.second};
            ErrHandler().CallErr(codeline, BREAK_CONTINUE_ONLY_ALLOWED_FOR, {});
          }
          if (res.second) {
            ignoreif = 1;
            continue;
          }
        }
        storages.erase(storages.begin());
        return {0, true};
      }

      case ElifStmt: {
        storages.insert(storages.begin(), Storage());
        Object condition = expression[0].Execute(storages);
        if (condition.GetBase()._t != BOOL) ErrHandler().CallErr(codeline, ELIF_NO_BOOLEAN_CONDITION, {});
        if (condition.GetBase().GetValue() == "0") {
          return {0, false};
        }
        bool ignoreif = 0;
        for (AST ast : childStmt) {
          if (ignoreif) {
            if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
            ignoreif = 0;
          }
          std::pair<int, bool> res = ast.Execute(storages);
          if (res.first >= 1) {
            return {res.first, res.second};
            ErrHandler().CallErr(codeline, BREAK_CONTINUE_ONLY_ALLOWED_FOR, {});
          }
          if (res.second) {
            ignoreif = 1;
            continue;
          }
        }
        storages.erase(storages.begin());
        return {0, true};
      }

      case ElseStmt: {
        storages.insert(storages.begin(), Storage());
        bool ignoreif = 0;
        for (AST ast : childStmt) {
          if (ignoreif) {
            if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
            ignoreif = 0;
          }
          std::pair<int, bool> res = ast.Execute(storages);
          if (res.first >= 1) {
            return {res.first, res.second};
            ErrHandler().CallErr(codeline, BREAK_CONTINUE_ONLY_ALLOWED_FOR, {});
          }
          if (res.second) {
            ignoreif = 1;
            continue;
          }
        }
        storages.erase(storages.begin());
        return {0, true};
      }

      case ForStmt: {
        if (argument.size() >= 1) {
          if (argument[0].GetBase().GetValue() == "C" && argument.size() == 2) {  // for clause
            for (int idx = std::stoi(expression[0].Execute(storages).GetBase().GetValue()); idx < std::stoi(expression[1].Execute(storages).GetBase().GetValue()); idx += std::stoi(expression[2].Execute(storages).GetBase().GetValue())) {
              storages.insert(storages.begin(), Storage());
              storages[0][argument[1].GetBase().GetValue()] = Object(argument[1].GetBase().GetValue(), {}, {}, Variable(argument[1].GetBase().GetValue(), std::to_string(idx), INT), 0, codeline, OK);
              bool ignoreif = 0;
              bool flowstmt = 0;
              for (AST ast : childStmt) {
                if (ignoreif) {
                  if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
                  ignoreif = 0;
                }
                std::pair<int, bool> res = ast.Execute(storages);
                if (res.first == 2) {
                  flowstmt = 1;
                  break;
                }
                if (res.first == 1) break;
                if (res.second) {
                  ignoreif = 1;
                  continue;
                }
              }
              storages.erase(storages.begin());
              if (flowstmt) break;
            }
          } else {  // single condition
            while (1) {
              storages.insert(storages.begin(), Storage());
              Object condition = expression[0].Execute(storages);
              if (condition.GetBase()._t != BOOL) ErrHandler().CallErr(codeline, FOR_NO_BOOLEAN_CONDITION, {});
              if (condition.GetBase().GetValue() == "0") break;

              bool ignoreif = 0;
              bool flowstmt = 0;
              for (AST ast : childStmt) {
                if (ignoreif) {
                  if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
                  ignoreif = 0;
                }
                std::pair<int, bool> res = ast.Execute(storages);
                if (res.first == 2) {
                  flowstmt = 1;
                  break;
                }
                if (res.first == 1) break;
                if (res.second) {
                  ignoreif = 1;
                  continue;
                }
              }
              storages.erase(storages.begin());
              if (flowstmt) break;
            }
          }
          break;
        }
      }
    }
    return {0, false};
  }
};

#endif