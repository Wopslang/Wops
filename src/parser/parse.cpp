/*
 * src/parser/parse.cpp
 * Wopslang Parsing Sourcefile
 *
 * 2021, Wops Team
 *
 * */

#include "parse.h"

std::vector<char> oprs{
    '+', '-', '*', '/', '%', '=', '>', '<', '!', '&', '|', '(', ')', '[', ']', ':', ';', ',', '?', '$', '~',
};

std::vector<String> operators{
    "+", "-", "*", "/", "%", "=", "==", "!=", ">", "<", ">=", "<=", "!", "&&", "||", "//"
};

std::vector<std::pair<String, String>> runes{
    {"\a", "a"}, {"\b", "b"}, {"\f", "f"}, {"\n", "n"}, {"\r", "r"}, {"\t", "t"}, {"\v", "v"}, {"\\", "\\"}, {"\'", "'"}, {"\"", "\""}
};

// for test
std::vector<String> funcs {"in", "out", "tostring", "toint"};

// Expr ParseExpr(std::vector<String> tokens, int parsing_line)
// Group string tokens into Expr structure
// Expr ParseExpr(std::vector<String> tokens, int parsing_line) {
//     // :TODO update
//     Expr head({0,0,0}, Variable("_", "", OPERATOR), parsing_line);
//     return head;
// }

// for test
Expr ParseExpr(std::vector<String> tokens, int parsing_line) {
    Expr head({0,0,0}, Variable("_", "", OPERATOR), parsing_line);

    if (tokens.size() >= 3 && std::find(funcs.begin(), funcs.end(), tokens[0]) != funcs.end() && 
            tokens[1] == "(" && tokens[tokens.size()-1] == ")") {
        int level = 0; bool isSuitable = 1;
        for (int idx = 1; idx < tokens.size(); idx++) {
            String token = tokens[idx];
            if (token == "(") level++;
            else if (token == ")") level--;
            else if (!level) isSuitable = 0;
        }
        if (isSuitable) {
            head = Expr({0,0,1}, Variable("_", tokens[0], OPERATOR), parsing_line);
            std::vector<String> parameter;
            for (int idx = 2; idx < tokens.size()-1; idx++) {
                if (tokens[idx] == ",") {
                    if (parameter.size() == 0) ErrHandler().CallErrDE(parsing_line, "Blank parameter");
                    head.AddChild(ParseExpr(parameter, parsing_line));
                    parameter.clear();
                    continue;
                }
                parameter.push_back(tokens[idx]);
            }
            if (parameter.size() != 0)
                head.AddChild(ParseExpr(parameter, parsing_line));
            return head;
        }
    }

    if (tokens[0] == "(" && tokens[tokens.size()-1] == ")") {
        int level = 0; bool isSuitable = 1;
        for (int idx = 0; idx < tokens.size(); idx++) {
            String token = tokens[idx];
            if (token == "(") level++;
            else if (token == ")") level--;
            else if (!level) isSuitable = 0;
        }
        if (isSuitable)
            tokens = std::vector<String>(tokens.begin()+1, tokens.end()-1);
    }

    if (tokens.size() == 0) ErrHandler().CallErrDE(parsing_line, "No operand");
    if (tokens.size() == 1) {
        if (std::regex_match(tokens[0], std::regex("[0-9]+"))) {
            head = Expr({1, 0, 0}, Variable("_", tokens[0], INT), parsing_line);
        } else if (std::regex_match(tokens[0], std::regex("[0-9]+.[0-9]+"))) {
            head = Expr({1, 0, 0}, Variable("_", tokens[0], DOUBLE), parsing_line);
        } else if (tokens[0][0] == '\"' && tokens[0][tokens[0].length()-1] == '\"') {
            head = Expr({1, 0, 0}, Variable("_", tokens[0], STRING), parsing_line);
        } else {
            head = Expr({0, 1, 0}, Variable("_", tokens[0], OPERATOR), parsing_line);
        }
        return head;
    }

    std::vector<bool> isTarget(tokens.size(), 0);
    int level = 0;
    for (int idx = 0; idx < tokens.size(); idx++) {
        String token = tokens[idx];
        if (token == "(") level++;
        else if (token == ")") level--;
        else if (!level) isTarget[idx] = 1;
    }
    if (level != 0) ErrHandler().CallErrDE(parsing_line, "Unmatched parenthesis");
    
    // priority 1
    for (int idx = 0; idx < tokens.size(); idx++) {
        if (!isTarget[idx]) continue;
        String token = tokens[idx];
        if (token == "||") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator || cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "||", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
    }

    // priority 2
    for (int idx = 0; idx < tokens.size(); idx++) {
        if (!isTarget[idx]) continue;
        String token = tokens[idx];
        if (token == "&&") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator && cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "&&", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
    }

    // priority 3
    for (int idx = 0; idx < tokens.size(); idx++) {
        if (!isTarget[idx]) continue;
        String token = tokens[idx];
        if (token == "==") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator == cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "==", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "!=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator != cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "!=", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "<") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator < cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "<", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "<=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator <= cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "<=", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == ">") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator > cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", ">", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == ">=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator >= cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", ">=", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
    } 

    // priority 4
    for (int idx = 0; idx < tokens.size(); idx++) {
        String token = tokens[idx];
        if (!isTarget[idx]) continue;
        if (token == "+") {
            if (idx != 0 && std::find(operators.begin(), operators.end(), tokens[idx - 1]) != operators.end())
                continue;
            if (idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator + cannot be unary");
            if (idx == 0) {
                if (tokens.size() != 2) ErrHandler().CallErrDE(parsing_line, "invalid unary operation form");
                if (std::regex_match(tokens[1], std::regex("[0-9]+"))) {
                    head = Expr({1, 0, 0}, Variable("_", tokens[1], INT), parsing_line);
                } else if (std::regex_match(tokens[1], std::regex("[0-9]+.[0-9]+"))) {
                    head = Expr({1, 0, 0}, Variable("_", tokens[1], DOUBLE), parsing_line);
                } else if (tokens[1][0] == '\"' && tokens[1][tokens[1].length()-1] == '\"') {
                    ErrHandler().CallErrDE(parsing_line, "operator + in unary use cannot be used with string constant");
                } else {
                    ErrHandler().CallErrDE(parsing_line, "operator + in unary use cannot be used with lvalue");
                }
                return head;
            }
            head = Expr({0, 0, 0}, Variable("_", "+", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "-") {
            if (idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator - cannot be unary");
            if (std::find(operators.begin(), operators.end(), tokens[idx - 1]) != operators.end())
                continue;
            if (idx == 0) {
                if (tokens.size() != 2) ErrHandler().CallErrDE(parsing_line, "invalid unary operation form");
                if (std::regex_match(tokens[1], std::regex("[0-9]+"))) {
                    head = Expr({1, 0, 0}, Variable("_", "-" + tokens[1], INT), parsing_line);
                } else if (std::regex_match(tokens[1], std::regex("[0-9]+.[0-9]+"))) {
                    head = Expr({1, 0, 0}, Variable("_", "-" + tokens[1], DOUBLE), parsing_line);
                } else if (tokens[1][0] == '\"' && tokens[1][tokens[1].length()-1] == '\"') {
                    ErrHandler().CallErrDE(parsing_line, "operator - in unary use cannot be used with string constant");
                } else {
                    ErrHandler().CallErrDE(parsing_line, "operator - in unary use cannot be used with lvalue");
                }
                return head;
            }
            head = Expr({0, 0, 0}, Variable("_", "-", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
    }

    // priority 5
    for (int idx = 0; idx < tokens.size(); idx++) {
        String token = tokens[idx];
        if (!isTarget[idx]) continue;
        if (token == "*") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator * cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "*", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "/") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator / cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "/", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "%") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator % cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "%", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
    }

    // priority 6
    for (int idx = 0; idx < tokens.size(); idx++) {
        String token = tokens[idx];
        if (!isTarget[idx]) continue;
        if (token == "!") {
            if (idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator ! cannot appear after identifier");
            head = Expr({0, 0, 0}, Variable("_", "!", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr({tokens[idx+1]}, parsing_line)
            });
            return head;
        }
    }

    ErrHandler().CallErrDE(parsing_line, "invalid expression.");
}

// std::vector<String> GetTokenTable(String code)
// get token table from the string code (a single code line)
std::vector<String> GetTokenTable(String code) {
    // :TODO add some exceptions of the runes
    bool is_searching_string = false;
    bool is_searching_blank = false;
    std::vector<String> token_table;
    for (int idx = 0; idx < code.length(); idx++) {
        if (code[idx] == ' ' && !is_searching_string) {
            if (is_searching_blank) {
                code.erase(code.begin()+idx); idx--;
                continue;
            }
            is_searching_blank = true;

            String token; token.resize(idx);

            if (idx != 0) {
                std::copy(code.begin(), code.begin()+idx, token.begin());
                token_table.push_back(token); 
            }

            code.erase(0, idx+1);
            idx = -1;
            continue;
        } else is_searching_blank = false;
        if (code[idx] == '"') {
            is_searching_string = !(is_searching_string);
            if (!is_searching_string) {
                String token; token.resize(idx+1);
                std::copy(code.begin(), code.begin()+idx+1, token.begin());
                token_table.push_back(token); 

                code.erase(0, idx+1);
                idx = -1; continue;
            }
            String token; token.resize(idx);
            std::copy(code.begin(), code.begin()+idx, token.begin());
            token_table.push_back(token); 

            code.erase(0, idx);
            idx = 0;
            continue;
        }
        if (std::find(oprs.begin(), oprs.end(), code[idx]) != oprs.end() && !is_searching_string) {
            bool alreadyChecked = false;
            switch (code[idx]) {
                case '=': 
                case '!': 
                case '<': 
                case '>': {
                    if (code.length() > idx+1 && code[idx+1] == '=') {
                        String token; token.resize(idx);

                        if (idx != 0) {
                            std::copy(code.begin(), code.begin()+idx, token.begin());
                            token_table.push_back(token); 
                        }
                        
                        token.resize(2); std::copy(code.begin()+idx, code.begin()+idx+2, token.begin());
                        token_table.push_back(token);
                        
                        code.erase(0, idx+2);
                        idx = 0;
                        alreadyChecked = true;
                    }
                    break;
                }
                case '&': {
                    if (code.length() > idx+1 && code[idx+1] == '&') {
                        String token; token.resize(idx);

                        if (idx != 0) {
                            std::copy(code.begin(), code.begin()+idx, token.begin());
                            token_table.push_back(token); 
                        }
                        
                        token.resize(2); std::copy(code.begin()+idx, code.begin()+idx+2, token.begin());
                        token_table.push_back(token);
                        
                        code.erase(0, idx+2);
                        idx = 0;
                        alreadyChecked = true;
                    }
                    break;
                }
                case '|': {
                    if (code.length() > idx+1 && code[idx+1] == '|') {
                        String token; token.resize(idx);

                        if (idx != 0) {
                            std::copy(code.begin(), code.begin()+idx, token.begin());
                            token_table.push_back(token); 
                        }
                        
                        token.resize(2); std::copy(code.begin()+idx, code.begin()+idx+2, token.begin());
                        token_table.push_back(token);
                        
                        code.erase(0, idx+2);
                        idx = 0;
                        alreadyChecked = true;
                    }
                    break;
                }
                case '/': {
                    if (code.length() > idx+1 && code[idx+1] == '/') {
                        String token; token.resize(idx);

                        if (idx != 0) {
                            std::copy(code.begin(), code.begin()+idx, token.begin());
                            token_table.push_back(token); 
                        }
                        
                        token.resize(2); std::copy(code.begin()+idx, code.begin()+idx+2, token.begin());
                        token_table.push_back(token);
                        
                        code.erase(0, idx+2);
                        idx = 0;
                        alreadyChecked = true;
                    }
                    break;
                }
            }
            if (alreadyChecked) continue;
            
            String token; token.resize(idx);

            if (idx != 0) {
                std::copy(code.begin(), code.begin()+idx, token.begin());
                token_table.push_back(token); 
            }
            
            token = code[idx];
            token_table.push_back(token);
            
            code.erase(0, idx+1);
            idx = 0;
            continue;
        }
    }
    if (code.length()) token_table.push_back(code);
    return token_table;
}

// int Parse(AST& head, std::vector<String> codes)
// ** return value is the end of line of parsing block **
int Parse(AST& head, std::vector<String> codes) {
    int parsing_line = head.codeline; // base parsing line = 0
    for (int idx = 0; idx < codes.size(); idx++) {
        String code = codes[idx];
        parsing_line++;
        std::vector<String> token_table = GetTokenTable(code);

        // blank statement
        if (!token_table.size()) continue;
        
        // comment
        if (token_table[0] == "//") continue;

        // end line of the parsing block
        if (token_table[0] == ";") return parsing_line;

        // if statement
        if (token_table[0] == "if") {
            if (token_table[token_table.size()-1] != "?")
                ErrHandler().CallErr(parsing_line, NO_MATCHING_SYNTAX_IF, {});

            AST if_block(IfStmt, {}, {
                ParseExpr(std::vector<String>(token_table.begin()+1, token_table.end()-1), parsing_line)
            }, parsing_line);

            int end_block = Parse(
                    if_block, 
                    std::vector<String>(codes.begin()+parsing_line+1, codes.end())
            );

            head.AddChild(if_block);

            idx += end_block - parsing_line;
            
            // check elif statement
            parsing_line = end_block;
            
            std::vector<String> end_block_token_table = GetTokenTable(codes[idx]);
            if (end_block_token_table.size() == 1) continue;
            idx--; parsing_line--;  // need to check
        }
        
        // elif & else statement
        else if (token_table[0] == ";") {
            if (token_table.size() != 1 && token_table[token_table.size()-1] != "?")
                ErrHandler().CallErr(parsing_line, NO_MATCHING_SYNTAX_ELIF, {});
            if (token_table.size() == 2) {  // else statement
                AST else_block(ElifStmt, {}, {
                    ParseExpr(
                        std::vector<String>(token_table.begin()+1, token_table.end()-1),
                        parsing_line
                    )
                }, parsing_line);

                int end_block = Parse(
                        else_block, 
                        std::vector<String>(codes.begin()+parsing_line+1, codes.end())
                );

                head.AddChild(else_block);

                idx += end_block - parsing_line;
                
                // check elif statement
                parsing_line = end_block;
                
                std::vector<String> end_block_token_table = GetTokenTable(codes[idx]);
                if (end_block_token_table.size() == 1) continue;
                idx--; parsing_line--;  // need to check
                continue;
            }
            AST elif_block(ElifStmt, {}, {
                ParseExpr(
                    std::vector<String>(token_table.begin()+1, token_table.end()-1),
                    parsing_line
                )
            }, parsing_line);

            int end_block = Parse(
                    elif_block, 
                    std::vector<String>(codes.begin()+parsing_line+1, codes.end())
            );

            head.AddChild(elif_block);

            idx += end_block - parsing_line;
            
            // check elif statement
            parsing_line = end_block;
            
            std::vector<String> end_block_token_table = GetTokenTable(codes[idx]);
            if (end_block_token_table.size() == 1) continue;
            idx--; parsing_line--;  // need to check
        }

        // for statement
        else if (token_table[0] == "for") {
            if (token_table[token_table.size()-1] != "$") 
                ErrHandler().CallErr(parsing_line, NO_MATCHING_SYNTAX_FOR, {});

            // for statement with for clause
            if (std::find(token_table.begin(), token_table.end(), "in") != token_table.end()) {
                if (token_table.size() < 9 || token_table[2] != "in")
                    ErrHandler().CallErr(parsing_line, NO_MATCHING_SYNTAX_FOR, {});

                // check range
                std::vector<int> rangeidx_list;
                for (int rangeidx = 3; rangeidx < token_table.size()-1; rangeidx++)
                    if (token_table[rangeidx] == "~") {
                        rangeidx_list.push_back(rangeidx);
                    }

                // grammar checking (ex. 1~2 (x), 1~~2 (x), 3~5~1 (o))
                if (rangeidx_list.size() != 2 || 
                        rangeidx_list[0] - 2 == 1 ||
                        rangeidx_list[1] - rangeidx_list[0] == 1 ||
                        token_table.size()-1 - rangeidx_list[1] == 1)
                    ErrHandler().CallErr(parsing_line, NO_MATCHING_SYNTAX_FOR, {});

                AST for_block(ForClauseStmt, {
                        Variable("_", token_table[1], OPERATOR),
                    }, {
                        ParseExpr(std::vector<String>(token_table.begin()+3, token_table.begin()+rangeidx_list[0]), parsing_line),
                        ParseExpr(std::vector<String>(token_table.begin()+rangeidx_list[0]+1, token_table.begin()+rangeidx_list[1]), parsing_line),
                        ParseExpr(std::vector<String>(token_table.begin()+rangeidx_list[1]+1, token_table.end()-1), parsing_line),
                    }, parsing_line);

                int end_block = Parse(
                        for_block, 
                        std::vector<String>(codes.begin()+parsing_line+1, codes.end())
                );

                head.AddChild(for_block);

                idx += end_block - parsing_line;
                parsing_line = end_block;
                continue;
            } 

            // for statement with single condition
            AST for_block(ForSCStmt, {}, {
                ParseExpr(std::vector<String>(token_table.begin()+1, token_table.end()-2), parsing_line),
            }, parsing_line);

            int end_block = Parse(
                for_block,
                std::vector<String>(codes.begin()+parsing_line+1, codes.end())
            );

            head.AddChild(for_block);

            idx += end_block - parsing_line;
            parsing_line = end_block;
        }

        // break statement
        else if (token_table[0] == "break") {
            if (token_table.size() > 1)
                    ErrHandler().CallErr(parsing_line, NO_MATCHING_SYNTAX_FOR, {});

            AST break_stmt(BreakStmt, {}, {}, parsing_line);
            head.AddChild(break_stmt);
        }

        // continue statement
        else if (token_table[0] == "continue") {
            if (token_table.size() > 1)
                    ErrHandler().CallErr(parsing_line, NO_MATCHING_SYNTAX_CONTINUE, {});
            AST continue_stmt(ContinueStmt, {}, {}, parsing_line);
            head.AddChild(continue_stmt);
        }

        // Assignment
        else if (token_table.size() > 2 && token_table[1] == "=") {
            AST assignment(Assignment, {
                Variable("_", token_table[0], OPERATOR)
            }, { 
                ParseExpr(std::vector<String>(token_table.begin()+2, token_table.end()), parsing_line)
            }, parsing_line);

            head.AddChild(assignment);
        }

        // VarDel
        else if (token_table.size() > 3 && token_table[2] == "=") {
            AST vardel(Assignment, {
                Variable("_", token_table[0], OPERATOR),
                Variable("_", token_table[1], OPERATOR)
            }, { 
                ParseExpr(std::vector<String>(token_table.begin()+3, token_table.end()), parsing_line)
            }, parsing_line);

            head.AddChild(vardel);
        }

        // ConstDel
        else if (token_table.size() > 4 && token_table[0] == "const" && token_table[3] == "=") {
            AST constdel(ConstDel, {
                Variable("_", token_table[1], OPERATOR),
                Variable("_", token_table[2], OPERATOR)
            }, { 
                ParseExpr(std::vector<String>(token_table.begin()+4, token_table.end()), parsing_line)
            }, parsing_line);

            head.AddChild(constdel);
        }

        // Expression
        else {
            AST expression(Expression, {}, {
                ParseExpr(token_table, parsing_line)
            }, parsing_line);
            
            head.AddChild(expression);
        }
    }
}