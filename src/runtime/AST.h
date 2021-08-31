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
#include "../import_/eexec_.h"
#include "../type/variable.h"
#include "../type/array.h"
#include "../type/operator.h"

enum StmtType {
	Main,
	Declaration,
	Expression,
	Assignment,
	BreakStmt,
	ContinueStmt,
	IfStmt,
	ForStmt,
};

// :TODO Add some comments
class Expr {
	private:
	bool constant = 0, variable = 0, call = 0;
	Variable token;
	std::vector<Expr> children;

	public:
	Expr(std::vector<bool> t, Variable &tkn) {
		token = tkn, constant = t[0], variable = t[1], call = t[2];
	}

	void SetChildren(std::vector<Expr> &rplcment) {
		children = rplcment;
	}

	void AddChild(Expr &child) {
		children.push_back(child);
	}

	Variable Execute(std::unordered_map<std::string, Variable> &storage) {
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

class AST {
	private:
	StmtType _t;
	std::vector<AST> childStmt;
	std::vector<Expr> expression;
	std::vector<Variable> argument;
	std::unordered_map<std::string, Variable> storage;

	public:
	AST(StmtType t, std::vector<Variable> argv, std::vector<Expr> expr) {
		_t = t, argument = argv, expression = expr;
	}

	void SetChildren(std::vector<AST> rplcment) {
		childStmt = rplcment;
	}

	void AddChild(AST child) {
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

	void Execute() {
		// :TODO Implement it
	}
};

#endif