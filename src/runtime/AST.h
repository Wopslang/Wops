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
	ForClauseStmt,
	ForSCStmt, // For statement with single condition
};

typedef std::unordered_map<std::string, Variable> Storage;

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
	Variable token;
	std::vector<Expr> children;
	int codeline;

	public:
	// constructor
	Expr(std::vector<bool> t, Variable tkn, int _codeline) {
		token = tkn, constant = t[0], variable = t[1], call = t[2], codeline = _codeline;
	}

	void SetChildren(std::vector<Expr> rplcment) {
		children = rplcment;
	}

	void AddChild(Expr child) {
		children.push_back(child);
	}

	// execute from the tree root
	Variable Execute(std::vector<Storage>& storages) {
		std::string tkn = token.GetValue();

		if (variable) {
			auto iter = storages[0].find(tkn);
			bool wasErrorOccured = true;
			for (Storage storage: storages) {
				iter = storage.find(tkn);
				if (iter != storage.end()) {
					wasErrorOccured = false;
					break;
				}
			}
			if (wasErrorOccured)  // has variable declared?
				ErrHandler().CallErr(codeline, tkn + " has not declared yet");

			return iter->second;
		}
		if (constant) {
			return token;
		}
		if (call) {
			Array arg;
			for (Expr child: children) {
				arg.container.push_back(child.Execute(storages));
			}
			ArrayWithCode res = EExecFunc(tkn, arg);
			if (res.error == ERROR)
				ErrHandler().CallErr(codeline, "Error occured while calling " + tkn);
			
			return res.var.container[0];
		}

		// Binary Operation
		if (tkn == "+") {
			return children[0].Execute(storages) + children[1].Execute(storages);
		}
		if (tkn == "-") {
			return children[0].Execute(storages) - children[1].Execute(storages);
		}
		if (tkn == "*") {
			return children[0].Execute(storages) * children[1].Execute(storages);
		}
		if (tkn == "/") {
			return children[0].Execute(storages) / children[1].Execute(storages);
		}
		if (tkn == "%") {
			return children[0].Execute(storages) % children[1].Execute(storages);
		}
		if (tkn == "==") {
			return children[0].Execute(storages) == children[1].Execute(storages);
		}
		if (tkn == "!=") {
			return children[0].Execute(storages) != children[1].Execute(storages);
		}
		if (tkn == ">") {
			return children[0].Execute(storages) > children[1].Execute(storages);
		}
		if (tkn == ">=") {
			return children[0].Execute(storages) >= children[1].Execute(storages);
		}
		if (tkn == "<") {
			return children[0].Execute(storages) < children[1].Execute(storages);
		}
		if (tkn == "<=") {
			return children[0].Execute(storages) <= children[1].Execute(storages);
		}
		if (tkn == "||") {
			return children[0].Execute(storages) || children[1].Execute(storages);
		}
		if (tkn == "&&") {
			return children[0].Execute(storages) && children[1].Execute(storages);
		}

		// Unary Operation
		if (tkn == "!") {
			return !children[0].Execute(storages);
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
	std::vector<Variable> argument;

	public:
	int codeline;

	// constructor
	AST(StmtType t, std::vector<Variable> argv, std::vector<Expr> expr, int _codeline) {
		_t = t, argument = argv, expression = expr, codeline = _codeline;
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

	// storages: smaller index, inner variable
	std::pair<int, bool> Execute(std::vector<Storage>& storages) {
		switch (_t) {
			case Main: {
				bool ignoreif = 0;
				for (AST ast: childStmt) {
					if (ignoreif) {
						if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
						ignoreif = 0;
					}

					std::pair<int, bool> res = ast.Execute(storages);
					if (res.first >= 1)
						ErrHandler().CallErr(codeline, "break and continue statement only allowed to be used in for statements");

					if (res.second) {
						ignoreif = 1;
						continue;
					}
				}
				break;
			}

			case ConstDel: {
				Variable v_type = argument[0], v_identifier = argument[1];
				for (Storage& storage: storages) {
					if (storage.find(v_identifier.GetValue()) != storage.end())
						ErrHandler().CallErr(codeline, "Redefine variable " + v_identifier.GetValue());
				}
				
				TYPE v_t = v_type.GetValue() == "int" ? INT : (
					v_type.GetValue() == "bool" ? BOOL : (
						v_type.GetValue() == "string" ? STRING : DOUBLE
					)
				);

				storages[0].insert({
					v_identifier.GetValue(),
					Variable(
						v_identifier.GetValue(),
						expression[0].Execute(storages).GetValue(),
						v_t,
					true)
				});

				break;
			}

			case VarDel: {
				Variable v_type = argument[0], v_identifier = argument[1];
				for (Storage& storage: storages) {
					if (storage.find(v_identifier.GetValue()) != storage.end())
						ErrHandler().CallErr(codeline, "Redefine variable " + v_identifier.GetValue());
				}
				
				TYPE v_t = v_type.GetValue() == "int" ? INT : (
					v_type.GetValue() == "bool" ? BOOL : (
						v_type.GetValue() == "string" ? STRING : DOUBLE
					)
				);

				storages[0].insert({
					v_identifier.GetValue(),
					Variable(
						v_identifier.GetValue(),
						expression[0].Execute(storages).GetValue(),
						v_t,
					false)
				});
				break;
			}

			case Expression:
				expression[0].Execute(storages).GetValue();
				break;

			case Assignment: {
				Variable v_identifier = argument[0];
				bool wasErrorOccured = true;
				for (Storage& storage: storages)
					if (storage.find(v_identifier.GetValue()) != storage.end()) {
						if (storage[v_identifier.GetValue()].constant)
							ErrHandler().CallErr(codeline, v_identifier.GetValue() + " is constant");
						storage[v_identifier.GetValue()].Substitute(expression[0].Execute(storages).GetValue());
						wasErrorOccured = false;
						break;
					}
				if (wasErrorOccured)
					ErrHandler().CallErr(codeline, "Variable " + v_identifier.GetValue() + " hasn't defined yet");
				break;
			}

			case BreakStmt:
				return {2, false};

			case ContinueStmt:
				return {1, false};

			case IfStmt: {
				storages.insert(storages.begin(), Storage());
				Variable condition = expression[0].Execute(storages);
				if (condition._t != BOOL)
					ErrHandler().CallErr(codeline, "If Statement allows only boolean condition expression.");
				if (condition.GetValue() == "0") {
					return {0, false};
				}
				bool ignoreif = 0;
				for (AST ast: childStmt) {
					if (ignoreif) {
						if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
						ignoreif = 0;
					}
					std::pair<int, bool> res = ast.Execute(storages);
					if (res.first >= 1) {
					    return {res.first, res.second};
						ErrHandler().CallErr(codeline, "If Statement doesn't allow to use break or continue statement.");
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
				Variable condition = expression[0].Execute(storages);
				if (condition._t != BOOL)
					ErrHandler().CallErr(codeline, "Elif Statement allows only boolean condition expression.");
				if (condition.GetValue() == "0") {
					return {0, false};
				}
				bool ignoreif = 0;
				for (AST ast: childStmt) {
					if (ignoreif) {
						if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
						ignoreif = 0;
					}
					std::pair<int, bool> res = ast.Execute(storages);
					if (res.first >= 1) {
					    return {res.first, res.second};
						ErrHandler().CallErr(codeline, "Elif Statement doesn't allow to use break or continue statement.");
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
				for (AST ast: childStmt) {
					if (ignoreif) {
						if (ast._t == ElifStmt || ast._t == ElseStmt) continue;
						ignoreif = 0;
					}
					std::pair<int, bool> res = ast.Execute(storages);
					if (res.first >= 1) {
					    return {res.first, res.second};
						ErrHandler().CallErr(codeline, "Else Statement doesn't allow to use break or continue statement.");
					}
					if (res.second) {
						ignoreif = 1;
						continue;
					}
				}
				storages.erase(storages.begin());
				return {0, true};
			}

			case ForClauseStmt: {
				for (int idx = std::stoi(expression[0].Execute(storages).GetValue()); idx < std::stoi(expression[1].Execute(storages).GetValue()); idx += std::stoi(expression[2].Execute(storages).GetValue())) {
					storages.insert(storages.begin(), Storage());
					storages[0][argument[0].GetValue()] = Variable(argument[0].GetValue(), std::to_string(idx), INT);
					bool ignoreif = 0;
					bool flowstmt = 0;
					for (AST ast: childStmt) {
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
				break;
			}

			case ForSCStmt: {
				while (1) {
					storages.insert(storages.begin(), Storage());
					Variable condition = expression[0].Execute(storages);
					if (condition._t != BOOL)
						ErrHandler().CallErr(codeline, "For Statement allows only boolean condition expression.");
					if (condition.GetValue() == "0") break;

					bool ignoreif = 0;
					bool flowstmt = 0;
					for (AST ast: childStmt) {
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
				break;
			}
		}
		return {0, false};
	}
};

#endif
