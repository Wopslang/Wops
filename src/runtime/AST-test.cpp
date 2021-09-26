/*
 * src/runtime/AST-test.cpp
 * Wopslang AST test sourcefile
 *
 * This is the sample code that constructs collatz sequence simulator
 * 
 * How to
 * ---
 * 1. declare ASTs (there must be main AST)
 * 2. declare every tokens (in this code, some token expression nodes were reused, but we suggest you not to reuse tokens)
 *     - See the defination of Expr for more detail
 * 3. connect them
 *     - Connection Order: expression-expression, connected expression-AST, AST-AST
 * 4. declare empty variable storage and execute main AST
 * 
 * Running this code
 * ---
 * use make command to run this code
 * 
 * 2021, Wops Team
 * */

#include "AST.h"

/*
int a = toint(in());
for (a != 1) {
	if (a % 2 == 1) {
		a = 3 * a + 1
	} else {
		a = a / 2
	}
	out(tostring(a)+"\n")
}
*/

int main() {
	// 1.
	AST main_code(Main, {}, {});
	AST line1(VarDel, {Variable("_", "int", OPERATOR), Variable("_", "a", OPERATOR)}, {});
	AST line2(ForSCStmt, {}, {});
	AST line3(IfStmt, {}, {});
	AST line4(Assignment, {Variable("_", "a", OPERATOR)}, {});
	AST line5(ElseStmt, {}, {});
	AST line6(Assignment, {Variable("_", "a", OPERATOR)}, {});
	AST line8(Expression, {}, {});

	// 2.
	Expr tkn1({0, 0, 1}, Variable("_", "toint", OPERATOR));
	Expr tkn2({0, 0, 1}, Variable("_", "out", OPERATOR));
	Expr tkn3({0, 1, 0}, Variable("_", "a", OPERATOR));
	Expr tkn4({1, 0, 0}, Variable("_", "1", INT));
	Expr tkn5({1, 0, 0}, Variable("_", "2", INT));
	Expr tkn6({1, 0, 0}, Variable("_", "3", INT));
	Expr tkn7({0, 0, 0}, Variable("_", "!=", OPERATOR));
	Expr tkn8({0, 0, 0}, Variable("_", "%", OPERATOR));
	Expr tkn9({0, 0, 0}, Variable("_", "==", OPERATOR));
	Expr tkn10({0, 0, 0}, Variable("_", "+", OPERATOR));
	Expr tkn11({0, 0, 0}, Variable("_", "/", OPERATOR));
	Expr tkn12({0, 0, 1}, Variable("_", "in", OPERATOR));
	Expr tkn13({0, 0, 0}, Variable("_", "*", OPERATOR));
	Expr tkn14({0, 0, 0}, Variable("_", "+", OPERATOR));
	Expr tkn15({0, 0, 1}, Variable("_", "tostring", OPERATOR));
	Expr tkn16({1, 0, 0}, Variable("_", "\"\n\"", STRING));

	// 3.
	tkn1.AddChild(tkn12);
	tkn7.SetChildren({tkn3, tkn4});
	tkn8.SetChildren({tkn3, tkn5});
	tkn9.SetChildren({tkn8, tkn4});
	tkn13.SetChildren({tkn6, tkn3});
	tkn10.SetChildren({tkn13, tkn4});
	tkn11.SetChildren({tkn3, tkn5});
	tkn15.AddChild(tkn3);
	tkn14.SetChildren({tkn15, tkn16});
	tkn2.AddChild(tkn14);

	line1.SetExpr({tkn1});
	line2.SetExpr({tkn7});
	line3.SetExpr({tkn9});
	line4.SetExpr({tkn10});
	line6.SetExpr({tkn11});
	line8.SetExpr({tkn2});
	
	line3.AddChild(line4);
	line5.AddChild(line6);
	line2.SetChildren({line3, line5, line8});
	main_code.SetChildren({line1, line2});

	// 4.
	Storage empty;
	main_code.Execute(empty);
	return 0;
}
