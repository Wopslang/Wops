/*
 * src/parser/parse.cpp
 * Wopslang Parsing Sourcefile
 *
 * 2021, Wops Team
 *
 * now in test.
 *
 * */

#include "parse.h"

std::vector<char> oprs{
    '+', '-', '*', '/', '%', '=', '>', '<', '!', '&', '|', '(', ')', '[', ']', '{', '}', ',',
};

std::vector<String> funcs {"in", "out", "tostring", "toint"};

Expr ParseExpr(std::vector<String> tokens) {
    Expr head({0,0,0}, Variable("_", "", INT));

    // TODO: func calling expr
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

    if (tokens.size() == 0) ErrHandler().CallErr("No operand");
    if (tokens.size() == 1) {
        if (std::regex_match(tokens[0], std::regex("[0-9]+"))) {
            head = Expr({1, 0, 0}, Variable("_", tokens[0], INT));
        } else if (std::regex_match(tokens[0], std::regex("[0-9]+.[0-9]"))) {
            head = Expr({1, 0, 0}, Variable("_", tokens[0], DOUBLE));
        } else if (std::regex_match(tokens[0], std::regex("^\"(\\w+|\\W+)\"$"))) {
            head = Expr({1, 0, 0}, Variable("_", tokens[0], STRING));
        } else {
            head = Expr({0, 1, 0}, Variable("_", tokens[0], OPERATOR));
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
    if (level != 0) ErrHandler().CallErr("Unmatched parenthesis");
    
    // priority 1
    for (int idx = 0; idx < tokens.size(); idx++) {
        if (!isTarget[idx]) continue;
        String token = tokens[idx];
        if (token == "||") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator || cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "||", OPERATOR));
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
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator && cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "&&", OPERATOR));
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
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator == cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "==", OPERATOR));
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "!=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator != cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "!=", OPERATOR));
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "<") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator < cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "<", OPERATOR));
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "<=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator <= cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "<=", OPERATOR));
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == ">") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator > cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", ">", OPERATOR));
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == ">=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator >= cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", ">=", OPERATOR));
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
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator + cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "+", OPERATOR));
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "-") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator - cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "-", OPERATOR));
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
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator * cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "*", OPERATOR));
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "/") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator / cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "/", OPERATOR));
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx)),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()))
            });
            return head;
        }
        if (token == "%") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr("operator % cannot be unary");
            head = Expr({0, 0, 0}, Variable("_", "%", OPERATOR));
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
            if (idx == tokens.size()-1) ErrHandler().CallErr("operator ! cannot appear after identifier");
            head = Expr({0, 0, 0}, Variable("_", "!", OPERATOR));
            head.SetChildren({
                ParseExpr({tokens[idx+1]})
            });
            return head;
        }
    }
}

void Parse(AST& head, std::vector<String> codes) {
    for (int idx = 0; idx < codes.size(); idx++) {
        String code = codes[idx];

        size_t lwhitespace = code.find_first_not_of(" \n\r\t\f\v");
        code = (lwhitespace == String::npos) ? "" : code.substr(lwhitespace);

        size_t rwhitespace = code.find_last_not_of(" \n\r\t\f\v");
        code = (rwhitespace == String::npos) ? "" : code.substr(0, rwhitespace+1);

        std::vector<String> tokens;
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
                tokens.push_back(cache);
                cache = "";
                continue;
            }
            cache += letter;
        }
        switch (std::count(tokens.begin(), tokens.end(), "=")) {
            case 0: // break, continue, if, for stmt
                if (tokens.size() == 0) break;
                if (tokens.size() == 1 && tokens[0] == "//") break;
                if (tokens.size() == 1 && tokens[0] == "break") {
                    AST ast(BreakStmt, {}, {}); head.AddChild(ast);
                    break;
                }
                if (tokens.size() == 1 && tokens[0] == "continue") {
                    AST ast(ContinueStmt, {}, {}); head.AddChild(ast);
                }
                if (tokens.size() == 1 && tokens[0] == "{") {
                    return;
                }
                if (tokens[0] == "if") {
                    if (tokens.size() < 4 || tokens[1] != "(" || tokens[tokens.size()-2] != ")"
                            || tokens[tokens.size()-1] != "{")
                        ErrHandler().CallErr("No matching pattern: if");
                    AST ast(IfStmt, {},
                        {
                            ParseExpr(std::vector<String>(tokens.begin()+2, tokens.end()-2))
                        }
                    );
                    Parse(ast, std::vector<String>(codes.begin()+idx, codes.end()));
                }
                break;
            case 1: // declaration, assignment
                break;
            default: // error
                break;
        }
    }
}

int main() {
    std::unordered_map<String, Variable> stor;
    stor["a"] = Variable("a", "2", INT);
    std::cout << ParseExpr(
            {"(", "3", "+", "5", ")", "/", "(", "a", "*", "2", ")"}
    ).Execute(stor).GetValue();
//    AST codeAST(Main, {}, {});
//    Parse(codeAST, {"string a = \"Hello, \" + in()"});
}
