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
#include "../type/variable.h"
#include "../type/array.h"
#include "../type/operator.h"

enum CommandType {
	HEAD,
	FOR,
	FORWHILE,
	FORLIST,
	IF,
	ELIF,
	ELSE,
	EXPREND,
	EXEC,
};

class ExprAST {
	public:
	ExprAST() {
		op = NONE;
	}

	ExprAST(op_ oper, std::vector<std::string> argv_) {
		op = oper;
		argv = argv_;
	}

	void addChild(ExprAST child) {
		children.push_back(child);
	}

	op_ op;
	std::vector<std::string> argv;
	std::vector<ExprAST> children;
};

class AST {
	public:
	AST(CommandType com, ExprAST expr_) {
		comm = com;
		expr = expr_;
	}

	void addChild(AST child) {
		children.push_back(child);
	}

	CommandType comm;
	ExprAST expr;
	std::vector<AST> children;
};

#endif