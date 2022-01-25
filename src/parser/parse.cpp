/*
 * src/parser/parse.cpp
 * Wopslang Parsing Sourcefile
 *
 * 2021, Wops Team
 *
 * */

#include "parse.h"

std::vector<char> oprs{
    '+', '-', '*', '/', '%', '=', '>', '<', '!', '&', '|', '(', ')', '[', ']', '{', '}', ',',
};

std::vector<String> operators{
    "+", "-", "*", "/", "%", "=", "!=", ">", "<", ">=", "<=", "!", "&&", "||"
};

std::vector<std::pair<String, String>> runes{
    {"\a", "a"}, {"\b", "b"}, {"\f", "f"}, {"\n", "n"}, {"\r", "r"}, {"\t", "t"}, {"\v", "v"}, {"\\", "\\"}, {"\'", "'"}, {"\"", "\""}
};

std::vector<String> funcs {"in", "out", "tostring", "toint"};

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
                    if (parameter.size() == 0) ErrHandler().CallErr(parsing_line, "Blank parameter");
                    head.AddChild(ParseExpr(parameter));
                    parameter.clear();
                    continue;
                }
                parameter.push_back(tokens[idx]);
            }
            if (parameter.size() != 0)
                head.AddChild(ParseExpr(parameter));
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

    if (tokens.size() == 0) ErrHandler().CallErr(parsing_line, "No operand");
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
    if (level != 0) ErrHandler().CallErr(parsing_line, "Unmatched parenthesis");
    
    // priority 1
    for (int idx = 0; idx < tokens.size(); idx++) {
        if (!isTarget[idx]) continue;
        String token = tokens[idx];
        if (token == "||") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator || cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "||", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
    }

    // priority 2
    for (int idx = 0; idx < tokens.size(); idx++) {
        if (!isTarget[idx]) continue;
        String token = tokens[idx];
        if (token == "&&") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator && cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "&&", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
    }

    // priority 3
    for (int idx = 0; idx < tokens.size(); idx++) {
        if (!isTarget[idx]) continue;
        String token = tokens[idx];
        if (token == "==") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator == cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "==", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "!=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator != cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "!=", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "<") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator < cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "<", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "<=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator <= cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "<=", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == ">") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator > cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", ">", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == ">=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator >= cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", ">=", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
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
            if (idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator + cannot be unary");
            if (idx == 0) {
                if (tokens.size() != 2) ErrHandler().CallErr(parsing_line, "invalid unary operation form");
                if (std::regex_match(tokens[1], std::regex("[0-9]+"))) {
                    head = Expr({1, 0, 0}, Variable("_", tokens[1], INT), parsing_line);
                } else if (std::regex_match(tokens[1], std::regex("[0-9]+.[0-9]+"))) {
                    head = Expr({1, 0, 0}, Variable("_", tokens[1], DOUBLE), parsing_line);
                } else if (tokens[1][0] == '\"' && tokens[1][tokens[1].length()-1] == '\"') {
                    ErrHandler().CallErr(parsing_line, "operator + in unary use cannot be used with string constant");
                } else {
                    ErrHandler().CallErr(parsing_line, "operator + in unary use cannot be used with lvalue");
                }
                return head;
            }
            head = Expr({0, 0, 0}, Variable("_", "+", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "-") {
            if (idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator - cannot be unary");
            if (std::find(operators.begin(), operators.end(), tokens[idx - 1]) != operators.end())
                continue;
            if (idx == 0) {
                if (tokens.size() != 2) ErrHandler().CallErr(parsing_line, "invalid unary operation form");
                if (std::regex_match(tokens[1], std::regex("[0-9]+"))) {
                    head = Expr({1, 0, 0}, Variable("_", "-" + tokens[1], INT), parsing_line);
                } else if (std::regex_match(tokens[1], std::regex("[0-9]+.[0-9]+"))) {
                    head = Expr({1, 0, 0}, Variable("_", "-" + tokens[1], DOUBLE), parsing_line);
                } else if (tokens[1][0] == '\"' && tokens[1][tokens[1].length()-1] == '\"') {
                    ErrHandler().CallErr(parsing_line, "operator - in unary use cannot be used with string constant");
                } else {
                    ErrHandler().CallErr(parsing_line, "operator - in unary use cannot be used with lvalue");
                }
                return head;
            }
            head = Expr({0, 0, 0}, Variable("_", "-", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
    }

    // priority 5
    for (int idx = 0; idx < tokens.size(); idx++) {
        String token = tokens[idx];
        if (!isTarget[idx]) continue;
        if (token == "*") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator * cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "*", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "/") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator / cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "/", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "%") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator % cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "%", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
    }

    // priority 6
    for (int idx = 0; idx < tokens.size(); idx++) {
        String token = tokens[idx];
        if (!isTarget[idx]) continue;
        if (token == "!") {
            if (idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, "operator ! cannot appear after identifier");
            head = Expr({0, 0, 0}, Variable("_", "!", OPERATOR), parsing_line);
            head.SetChildren({
                ParseExpr({tokens[idx+1]})
            });
            return head;
        }
    }

    ErrHandler().CallErr(parsing_line, "invalid expression.");
}

void Parse(AST& head, std::vector<String> codes) {
    int parsing_line = head.codeline;
    std::vector<std::vector<String>> tokenss(codes.size());
    for (int idx = 0; idx < codes.size(); idx++) {
        String code = codes[idx];

        size_t lwhitespace = code.find_first_not_of(" \n\r\t\f\v");
        code = (lwhitespace == String::npos) ? "" : code.substr(lwhitespace);

        size_t rwhitespace = code.find_last_not_of(" \n\r\t\f\v");
        code = (rwhitespace == String::npos) ? "" : code.substr(0, rwhitespace+1);

        std::vector<String> &tokens = tokenss[idx];
        bool isParsingString = 0;
        String cache = "";

        for (int lidx = 0; lidx < code.length(); lidx++) {
            char letter = code[lidx];
            if (std::find(oprs.begin(), oprs.end(), letter) != oprs.end() && !isParsingString) {
                if (cache != "")
                    tokens.push_back(cache);
                cache = "";

                bool edited = 0;
                switch (letter) {
                    case '=':
                        if (lidx+1 != codes.size() && code[lidx+1] == '=') { // ==
                            edited = 1;
                            tokens.push_back("=="); lidx++;
                        }
                        break;
                    case '!':
                        if (lidx+1 != codes.size() && code[lidx+1] == '=') { // !=
                            edited = 1;
                            tokens.push_back("!="); lidx++;
                        }
                        break;
                    case '>':
                        if (lidx+1 != codes.size() && code[lidx+1] == '=') { // >=
                            edited = 1;
                            tokens.push_back(">="); lidx++;
                        }
                        break;
                    case '<':
                        if (lidx+1 != codes.size() && code[lidx+1] == '=') { // <=
                            edited = 1;
                            tokens.push_back("<="); lidx++;
                        }
                        break;
                    case '&':
                        if (lidx+1 != codes.size() && code[lidx+1] == '&') { // &
                            edited = 1;
                            tokens.push_back("&&"); lidx++;
                            break;
                        }
                        break;
                    case '|':
                        if (lidx+1 != codes.size() && code[lidx+1] == '|') { // |
                            edited = 1;
                            tokens.push_back("||"); lidx++;
                            break;
                        }
                        break;
                    case '/':
                        if (lidx+1 != codes.size() && code[lidx+1] == '/') { // //
                            edited = 1;
                            tokens.push_back("//"); lidx++;
                            break;
                        }
                }
                if (edited) continue;
                cache = letter;
                tokens.push_back(cache); cache = "";
                continue;
            }

            if (letter == ' ' && !isParsingString) {
                if (cache == "") continue;
                tokens.push_back(cache);
                cache = "";
                continue;
            }

            if (letter == '"') {
                isParsingString = !isParsingString;
                cache += letter;
                if (isParsingString) continue;
                
                for (std::pair<String, String> rune: runes) {
                    while (cache.find("\\" + rune.second) != String::npos) {
                        cache.replace(cache.find("\\" + rune.second), 2, rune.first);
                    }
                }
                tokens.push_back(cache);
                cache = "";
                continue;
            }
            cache += letter;
        }
        if (cache.size())
            tokens.push_back(cache);
    }

    for (int idx = 0; idx < tokenss.size(); idx++) {
        std::vector<String> tokens = tokenss[idx];
        switch (std::count(tokens.begin(), tokens.end(), "=")) {
            case 0: { // expression, break, continue, if, for stmt
                if (tokens.size() == 0) break;
                if (tokens.size() >= 1 && tokens[0] == "//") break;
                if (tokens.size() == 1 && tokens[0] == "break") {
                    AST ast(BreakStmt, {}, {}, parsing_line); head.AddChild(ast);
                    break;
                }
                if (tokens.size() == 1 && tokens[0] == "continue") {
                    AST ast(ContinueStmt, {}, {}, parsing_line); head.AddChild(ast);
                    break;
                }
                if (tokens.size() == 1 && tokens[0] == "}") {
                    return;
                }
                if (tokens[0] == "if") {
                    if (tokens.size() < 4 || tokens[1] != "(" || tokens[tokens.size()-2] != ")"
                            || tokens[tokens.size()-1] != "{")
                        ErrHandler().CallErr(parsing_line, "No matching syntax: if");

                    AST ast(IfStmt, {},
                        {
                            ParseExpr(std::vector<String>(tokens.begin()+2, tokens.end()-2))
                        }, parsing_line
                    );

                    int level = 0, iidx;
                    for (iidx = idx; iidx < tokenss.size(); iidx++) {
                        std::vector<String> ttokens = tokenss[iidx];
                        if (ttokens[0] == "for" || ttokens[0] == "if" || ttokens[0] == "elif" || ttokens[0] == "else")
                            level++;
                        if (ttokens[0] == "}") level--;
                        if (!level) break;
                    }
                    Parse(ast, std::vector<String>(codes.begin()+idx+1, codes.begin()+iidx));
                    head.AddChild(ast);
                    idx = iidx;
                    break;
                }
                if (tokens[0] == "elif") {
                    if (tokens.size() < 4 || tokens[1] != "(" || tokens[tokens.size()-2] != ")"
                            || tokens[tokens.size()-1] != "{")
                        ErrHandler().CallErr(parsing_line, "No matching syntax: elif");

                    AST ast(ElifStmt, {},
                        {
                            ParseExpr(std::vector<String>(tokens.begin()+2, tokens.end()-2))
                        }, parsing_line
                    );

                    int level = 0, iidx;
                    for (iidx = idx; iidx < tokenss.size(); iidx++) {
                        std::vector<String> ttokens = tokenss[iidx];
                        if (ttokens[0] == "for" || ttokens[0] == "if" || ttokens[0] == "elif" || ttokens[0] == "else")
                            level++;
                        if (ttokens[0] == "}") level--;
                        if (!level) break;
                    }
                    Parse(ast, std::vector<String>(codes.begin()+idx+1, codes.begin()+iidx));
                    head.AddChild(ast);
                    idx = iidx;
                    break;
                }
                if (tokens[0] == "else") {
                    if (tokens.size() != 2 || tokens[1] != "{")
                        ErrHandler().CallErr(parsing_line, "No matching syntax: else");

                    AST ast(ElseStmt, {}, {}, parsing_line);

                    int level = 0, iidx;
                    for (iidx = idx; iidx < tokenss.size(); iidx++) {
                        std::vector<String> ttokens = tokenss[iidx];
                        if (ttokens[0] == "for" || ttokens[0] == "if" || ttokens[0] == "elif" || ttokens[0] == "else")
                            level++;
                        if (ttokens[0] == "}") level--;
                        if (!level) break;
                    }
                    Parse(ast, std::vector<String>(codes.begin()+idx+1, codes.begin()+iidx));
                    head.AddChild(ast);
                    idx = iidx;
                    break;
                }
                if (tokens[0] == "for") {
                    if (tokens.size() < 4)
                        ErrHandler().CallErr(parsing_line, "No matching syntax: for");

                    if (std::find(tokens.begin(), tokens.end(), "range") != tokens.end()) { // for clause
                        if (tokens[2] != "in" ||
                            tokens[3] != "range" ||
                            tokens[4] != "(" ||
                            tokens[tokens.size()-1] != "{" ||
                            tokens[tokens.size()-2] != ")"
                            ) 
                            ErrHandler().CallErr(parsing_line, "No matching syntax: for");

                        AST ast(ForClauseStmt, {
                            Variable("_", tokens[1], OPERATOR), 
                        }, {}, parsing_line);

                        std::vector<String> caches;
                        int grammar_checknum = 0;
                        for (int iidx = 5; iidx < tokens.size()-2; iidx++) {
                            if (tokens[iidx] == ",") {
                                if (grammar_checknum > 3)
                                    ErrHandler().CallErr(parsing_line, "For clause only has three elements: start, end, step");

                                grammar_checknum++;
                                ast.AddExpr(ParseExpr(caches));
                                caches.clear();
                                continue;
                            }
                            caches.push_back(tokens[iidx]);
                        }
                        if (!caches.size()) {
                            ErrHandler().CallErr(parsing_line, "For clause doesn't allow blank expression");
                        }
                        ast.AddExpr(ParseExpr(caches));
                        caches.clear();
                        grammar_checknum++;

                        if (grammar_checknum != 3) 
                            ErrHandler().CallErr(parsing_line, "For clause only has three elements: start, end, step");

                        int level = 0, iidx;
                        for (iidx = idx; iidx < tokenss.size(); iidx++) {
                            std::vector<String> ttokens = tokenss[iidx];
                            if (ttokens[0] == "for" || ttokens[0] == "if" || ttokens[0] == "elif" || ttokens[0] == "else")
                                level++;
                            if (ttokens[0] == "}") level--;
                            if (!level) break;
                        }
                        Parse(ast, std::vector<String>(codes.begin()+idx+1, codes.begin()+iidx));
                        head.AddChild(ast);
                        idx = iidx;
                        break;
                    }

                    // ForSCStmt
                    if (tokens.size() < 4 || tokens[1] != "(" || tokens[tokens.size()-2] != ")"
                            || tokens[tokens.size()-1] != "{")
                        ErrHandler().CallErr(parsing_line, "No matching syntax: for");

                    AST ast(ForSCStmt, {},
                        {
                            ParseExpr(std::vector<String>(tokens.begin()+2, tokens.end()-2))
                        }, parsing_line
                    );

                    int level = 0, iidx;
                    for (iidx = idx; iidx < tokenss.size(); iidx++) {
                        std::vector<String> ttokens = tokenss[iidx];
                        if (ttokens[0] == "for" || ttokens[0] == "if" || ttokens[0] == "elif" || ttokens[0] == "else")
                            level++;
                        if (ttokens[0] == "}") level--;
                        if (!level) break;
                    }
                    Parse(ast, std::vector<String>(codes.begin()+idx+1, codes.begin()+iidx));
                    head.AddChild(ast);
                    idx = iidx;
                    break;
                }
                
                AST ast(Expression, {},
                    {
                        ParseExpr(tokens)
                    }, parsing_line
                );
                head.AddChild(ast);
                break;
            }

            case 1: { // declaration, assignment
                // WEAK TODO: change the way to identify Assignment and Declaration
                int pos = std::find(tokens.begin(), tokens.end(), "=") - tokens.begin();

                if (pos == 1) { // assignment
                    if (tokens.size() < 3)
                        ErrHandler().CallErr(parsing_line, "No matching syntax: assignment");

                    AST ast(Assignment,
                        {
                            Variable("_", tokens[0], OPERATOR)
                        }, {
                            ParseExpr(std::vector<String>(tokens.begin()+2, tokens.end()))
                        }, parsing_line
                    );
                    head.AddChild(ast);
                    break;
                }
                if (pos == 2) { // VarDel
                    if (tokens.size() < 4)
                        ErrHandler().CallErr(parsing_line, "No matching syntax: declaration");

                    AST ast(VarDel,
                        {
                            Variable("_", tokens[0], OPERATOR),
                            Variable("_", tokens[1], OPERATOR)
                        }, {
                            ParseExpr(std::vector<String>(tokens.begin()+3, tokens.end()))
                        }, parsing_line
                    );
                    head.AddChild(ast);
                    break;
                }
                if (pos == 3) { // ConstDel
                    if (tokens.size() < 5)
                        ErrHandler().CallErr(parsing_line, "No matching syntax: declaration");

                    AST ast(ConstDel,
                        {
                            Variable("_", tokens[1], OPERATOR),
                            Variable("_", tokens[2], OPERATOR)
                        }, {
                            ParseExpr(std::vector<String>(tokens.begin()+4, tokens.end()))
                        }, parsing_line
                    );
                    head.AddChild(ast);
                    break;
                }

                ErrHandler().CallErr(parsing_line, "Unknown syntax");
                break;
            }

            default: // error
                ErrHandler().CallErr(parsing_line, "Unknown syntax");
                break;
        }
    }
}

