/*
 * src/runtime/AST.h
 * Wopslang AST Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_AST_H
#define WOPS_AST_H

#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include "../import_/eexec_.h"
#include "../type/variable.h"
#include "../type/array.h"
#include "../type/operator.h"

enum StmtType {
	Main,
	ConstDel,
	VarDel,
	Expression,
	Assignment,
	BreakStmt,
	ContinueStmt,
	IfStmt,
	ForClauseStmt,
	ForSCStmt, // For statement with single condition
};

// :TODO Add some comments
class Expr {
	private:
	bool constant = 0, variable = 0, call = 0;
	Variable token;
	std::vector<Expr> children;

	public:
	Expr(std::vector<bool> t, Variable tkn) {
		token = tkn, constant = t[0], variable = t[1], call = t[2];
	}

	void SetChildren(std::vector<Expr> rplcment) {
		children = rplcment;
	}

	void AddChild(Expr child) {
		children.push_back(child);
	}

	Variable Execute(std::unordered_map<std::string, Variable> storage) {
		std::string tkn = token.GetValue();

		if (variable) {
			auto iter = storage.find(tkn);
			if (iter == storage.end())
				ErrHandler().CallErr(tkn + " has not declared yet");
			return iter->second;
		}
		if (constant) {
			return token;
		}
		if (call) {
			Array arg;
			for (Expr child: children) {
				arg.container.push_back(child.Execute(storage));
			}
			ArrayWithCode res = EExecFunc(tkn, arg);
			if (res.error == ERROR)
				ErrHandler().CallErr("Error occured while calling " + tkn);
			return res.var.container[0];
		}

		// Binary Operation
		if (tkn == "+") {
			return children[0].Execute(storage) + children[1].Execute(storage);
		}
		if (tkn == "-") {
			return children[0].Execute(storage) - children[1].Execute(storage);
		}
		if (tkn == "*") {
			return children[0].Execute(storage) * children[1].Execute(storage);
		}
		if (tkn == "/") {
			return children[0].Execute(storage) / children[1].Execute(storage);
		}
		if (tkn == "%") {
			return children[0].Execute(storage) - children[1].Execute(storage);
		}
		if (tkn == "==") {
			return children[0].Execute(storage) == children[1].Execute(storage);
		}
		if (tkn == "!=") {
			return children[0].Execute(storage) != children[1].Execute(storage);
		}
		if (tkn == ">") {
			return children[0].Execute(storage) > children[1].Execute(storage);
		}
		if (tkn == ">=") {
			return children[0].Execute(storage) >= children[1].Execute(storage);
		}
		if (tkn == "<") {
			return children[0].Execute(storage) < children[1].Execute(storage);
		}
		if (tkn == "<=") {
			return children[0].Execute(storage) <= children[1].Execute(storage);
		}

		// Unary Operation
		if (tkn == "!") {
			return !children[0].Execute(storage);
		}
	}
};

typedef std::unordered_map<std::string, Variable> Storage;

class AST {
	private:
	StmtType _t;
	std::vector<AST *> childStmt;
	std::vector<Expr> expression;
	std::vector<Variable> argument;

	public:
	AST(StmtType t, std::vector<Variable> argv, std::vector<Expr> expr) {
		_t = t, argument = argv, expression = expr;
	}

	void SetChildren(std::vector<AST *> rplcment) {
		childStmt = rplcment;
	}

	void AddChild(AST *child) {
		childStmt.push_back(child);
	}

	void SetExpr(std::vector<Expr> rplcment) {
		expression = rplcment;
	}

	void AddExpr(Expr expr) {
		expression.push_back(expr);
	}

	void SetArg(std::vector<Variable> argv) {
		argument = argv;
	}

	void AddArg(Variable argv) {
		argument.push_back(argv);
	}

	std::pair<bool, bool> Execute(Storage& storage) {
		switch (_t) {
			case Main:
				for (AST *child: childStmt) {
					child->Execute(storage);
				}
				break;

			case ConstDel: {
				Variable v_type = argument[0], v_identifier = argument[1];
				if (storage.find(v_identifier.GetValue()) != storage.end())
					ErrHandler().CallErr("Redefine variable " + v_identifier.GetValue());
				
				TYPE v_t = v_type.GetValue() == "int" ? INT : (
					v_type.GetValue() == "bool" ? BOOL : (
						v_type.GetValue() == "string" ? STRING : DOUBLE
					)
				);

				storage.insert({
					v_identifier.GetValue(),
					Variable(
						v_identifier.GetValue(),
						expression[0].Execute(storage).GetValue(),
						v_t,
					true)
				});

				break;
			}

			case VarDel: {
				Variable v_type = argument[0], v_identifier = argument[1];
				if (storage.find(v_identifier.GetValue()) != storage.end())
					ErrHandler().CallErr("Redefine variable " + v_identifier.GetValue());
				
				TYPE v_t = v_type.GetValue() == "int" ? INT : (
					v_type.GetValue() == "bool" ? BOOL : (
						v_type.GetValue() == "string" ? STRING : DOUBLE
					)
				);

				storage.insert({
					v_identifier.GetValue(),
					Variable(
						v_identifier.GetValue(),
						expression[0].Execute(storage).GetValue(),
						v_t,
					false)
				});
				break;
			}

			case Expression:
				expression[0].Execute(storage).GetValue();
				break;

			case Assignment: {
				Variable v_identifier = argument[0];
				if (storage.find(v_identifier.GetValue()) == storage.end())
					ErrHandler().CallErr("Variable " + v_identifier.GetValue() + " hasn't defined yet.");
				
				storage[v_identifier.GetValue()] = expression[0].Execute(storage);
				break;
			}

			case BreakStmt:
				return {true, false};

			case ContinueStmt:
				return {false, true};

			case IfStmt: {
				// :TODO impl it later
				break;
			}

			case ForClauseStmt: {
				break;
			}

			case ForSCStmt: {
				Storage local = storage;
				while (1) {
					Variable condition = expression[0].Execute(local);
					if (condition._t != BOOL)
						ErrHandler().CallErr("For Statement allows only boolean condition expression.");
					if (condition.GetValue() == "0") break;
					for (AST *ast: childStmt) {
						std::pair<bool, bool> res = ast->Execute(local);
						if (res.first) break;
						if (res.second) continue;
					}
				}
				for (auto iter = local.begin(); iter != local.end(); iter++) {
					if (storage.find(iter->first) == storage.end()) local.erase(iter);
				}
				storage = local;
				break;
			}
		}
	}
};

#endif