/*
 * src/parser/parse.cpp
 * Wopslang Parsing Sourcefile
 *
 * 2023, Wops Team
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

// TODO: improve library system
std::vector<String> funcs {"in", "out", "tostring", "toint"};

// TODO: develop this function
// Expr ParseExpr(std::vector<String> tokens, int parsing_line)
// Group string tokens into Expr structure
// Expr ParseExpr(std::vector<String> tokens, int parsing_line) {
//     // :TODO update
//     Expr head({0,0,0}, Variable("_", "", OPERATOR), parsing_line);
//     return head;
// }

// Deprecated
Expr ParseExpr(std::vector<String> tokens, int parsing_line) {
    Expr head({0,0,0},
        Object("_", {}, {}, Variable("_", "", OPERATOR), 0, parsing_line, OK), parsing_line);

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
            head = Expr({0,0,1}, Object("_", {}, {}, Variable("_", tokens[0], OPERATOR), 0, parsing_line, OK), parsing_line);
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
            head = Expr({1, 0, 0}, Object("_", {}, {}, Variable("_", tokens[0], INT), 0, parsing_line), parsing_line);
        } else if (std::regex_match(tokens[0], std::regex("[0-9]+.[0-9]+"))) {
            head = Expr({1, 0, 0}, Object("_", {}, {}, Variable("_", tokens[0], DOUBLE), 0, parsing_line), parsing_line);
        } else if (tokens[0][0] == '\"' && tokens[0][tokens[0].length()-1] == '\"') {
            head = Expr({1, 0, 0}, Object("_", {}, {}, Variable("_", tokens[0], STRING), 0, parsing_line), parsing_line);
        } else {
            head = Expr({0, 1, 0}, Object("_", {}, {}, Variable("_", tokens[0], OPERATOR), 0, parsing_line), parsing_line);
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
    if (level != 0) ErrHandler().CallErr(parsing_line, UNMATCHED_PARENTHESIS, {});
    
    // priority 1
    for (int idx = 0; idx < tokens.size(); idx++) {
        if (!isTarget[idx]) continue;
        String token = tokens[idx];
        if (token == "||") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, OPERATION_ONLY_BINARY, {"||"});
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "||", OPERATOR), 0, parsing_line), parsing_line);
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
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, OPERATION_ONLY_BINARY, {"&&"});
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "&&", OPERATOR), 0, parsing_line), parsing_line);
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
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, OPERATION_ONLY_BINARY, {"=="});
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "==", OPERATOR), 0, parsing_line), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "!=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, OPERATION_ONLY_BINARY, {"!="});
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "!=", OPERATOR), 0, parsing_line), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "<") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, OPERATION_ONLY_BINARY, {"<"});
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "<", OPERATOR), 0, parsing_line), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "<=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, OPERATION_ONLY_BINARY, {"<="});
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "<=", OPERATOR), 0, parsing_line), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == ">") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, OPERATION_ONLY_BINARY, {">"});
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", ">", OPERATOR), 0, parsing_line), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == ">=") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, OPERATION_ONLY_BINARY, {">="});
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", ">=", OPERATOR), 0, parsing_line), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
    } 

    // :TODO remove all CallErrDE()

    // priority 4
    for (int idx = 0; idx < tokens.size(); idx++) {
        String token = tokens[idx];
        if (!isTarget[idx]) continue;
        if (token == "+") {
            if (idx != 0 && std::find(operators.begin(), operators.end(), tokens[idx - 1]) != operators.end())
                continue;
            if (idx == tokens.size()-1) ErrHandler().CallErr(parsing_line, OPERATION_ONLY_BINARY, {"+"});
            if (idx == 0) {
                if (tokens.size() != 2) ErrHandler().CallErrDE(parsing_line, "invalid unary operation form");
                if (std::regex_match(tokens[1], std::regex("[0-9]+"))) {
                    head = Expr({1, 0, 0}, Object("_", {}, {}, Variable("_", tokens[1], INT), 0, parsing_line), parsing_line);
                } else if (std::regex_match(tokens[1], std::regex("[0-9]+.[0-9]+"))) {
                    head = Expr({1, 0, 0}, Object("_", {}, {}, Variable("_", tokens[1], DOUBLE), 0, parsing_line), parsing_line);
                } else if (tokens[1][0] == '\"' && tokens[1][tokens[1].length()-1] == '\"') {
                    ErrHandler().CallErrDE(parsing_line, "operator + in unary use cannot be used with string constant");
                } else {
                    ErrHandler().CallErrDE(parsing_line, "operator + in unary use cannot be used with lvalue");
                }
                return head;
            }
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "+", OPERATOR), 0, parsing_line), parsing_line);
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
                    head = Expr({1, 0, 0}, Object("_", {}, {}, Variable("_", "-" + tokens[1], INT), 0, parsing_line), parsing_line);
                } else if (std::regex_match(tokens[1], std::regex("[0-9]+.[0-9]+"))) {
                    head = Expr({1, 0, 0}, Object("_", {}, {}, Variable("_", "-" + tokens[1], DOUBLE), 0, parsing_line), parsing_line);
                } else if (tokens[1][0] == '\"' && tokens[1][tokens[1].length()-1] == '\"') {
                    ErrHandler().CallErrDE(parsing_line, "operator - in unary use cannot be used with string constant");
                } else {
                    ErrHandler().CallErrDE(parsing_line, "operator - in unary use cannot be used with lvalue");
                }
                return head;
            }
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "-", OPERATOR), 0, parsing_line), parsing_line);
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
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "*", OPERATOR), 0, parsing_line), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "/") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator / cannot be unary");
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "/", OPERATOR), 0, parsing_line), parsing_line);
            head.SetChildren({
                ParseExpr(std::vector<String>(tokens.begin(), tokens.begin()+idx), parsing_line),
                ParseExpr(std::vector<String>(tokens.begin()+idx+1, tokens.end()), parsing_line)
            });
            return head;
        }
        if (token == "%") {
            if (idx == 0 || idx == tokens.size()-1) ErrHandler().CallErrDE(parsing_line, "operator % cannot be unary");
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "%", OPERATOR), 0, parsing_line), parsing_line);
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
            head = Expr({0, 0, 0}, Object("_", {}, {}, Variable("_", "!", OPERATOR), 0, parsing_line), parsing_line);
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

                // rune exception
                for (std::pair<String, String> rune: runes) {
                    while (token.find("\\" + rune.second) != String::npos) {
                        token.replace(token.find("\\" + rune.second), 2, rune.first);
                    }
                }
                token_table.push_back(token); 

                code.erase(0, idx+1);
                idx = -1;
            }
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
                        idx = -1;
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
                        idx = -1;
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
                        idx = -1;
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
                        idx = -1;
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
            idx = -1;
            continue;
        }
    }
    if (code.length()) token_table.push_back(code);
    return token_table;
}

// int Parse(AST& head, std::vector<String> codes)
// ** return value is the end of line of parsing block **
std::pair<int, int> Parse(AST& head, std::vector<std::vector<String>> Token_table, int parse_start, int arg_idx, std::vector<String>& token_storage) {
    int parsing_line = head.codeline; // base parsing line = 0
    StmtType prev_stmt = Main;
    for (; parsing_line < Token_table.size(); parsing_line++) {
        std::vector<String> token_table = Token_table[parsing_line];

        for (int idx = parse_start; idx < token_table.size(); idx++) {
            if (token_table[idx] == "//") break;
            if (token_table[idx] == "(") {
                // BracketBlock is only for adding bracket-inside tokens into token_table.
                // Thus, bracket_block isn't added in AST
                AST bracket_block(BracketBlock, {}, {}, parsing_line);
                token_storage.push_back("(");

                std::pair<int, int> res = Parse(bracket_block, Token_table, idx+1, 0, token_storage);

                parsing_line = res.first;
                idx = res.second;
                continue;
            }
            if (token_table[idx] == ")") {
                return {parsing_line, idx};
            }
            switch (head.GetStmt()) {
                // if-family statement
                case Main: {
                    if (token_table[idx] == "if") {
                        AST if_block(IfStmt, {}, {}, parsing_line);
                        std::pair<int, int> res = Parse(if_block, Token_table, idx+1, 1, token_storage);
                        head.AddChild(if_block);

                        parsing_line = res.first;
                        idx = res.second;
                        prev_stmt = IfStmt;
                        token_storage = {}; // if not need -> remove
                        continue;
                    }
                    if (token_table[idx] == "?") {
                        if (prev_stmt == IfStmt) {
                            if (token_storage.empty()) {  // else
                                AST else_block(ElseStmt, {}, {}, parsing_line);
                                std::pair<int, int> res = Parse(else_block, Token_table, idx+1, 0, token_storage);
                                head.AddChild(else_block);

                                parsing_line = res.first;
                                idx = res.second;
                                prev_stmt = ElseStmt;
                                token_storage = {}; // if not need -> remove
                                continue;
                            } else { // elif
                                AST elif_block(ElifStmt, {}, {ParseExpr(token_storage, parsing_line)}, parsing_line);
                                token_storage = {};
                                std::pair<int, int> res = Parse(elif_block, Token_table, idx+1, 0, token_storage);
                                head.AddChild(elif_block);

                                parsing_line = res.first;
                                idx = res.second;
                                prev_stmt = ElifStmt;
                                token_storage = {}; // if not need -> remove
                                continue;
                            }
                        } else if (prev_stmt == ElifStmt) {
                            if (token_storage.empty()) {  // else
                                AST else_block(ElseStmt, {}, {}, parsing_line);
                                std::pair<int, int> res = Parse(else_block, Token_table, idx+1, 0, token_storage);
                                head.AddChild(else_block);

                                parsing_line = res.first;
                                idx = res.second;
                                prev_stmt = ElseStmt;
                                token_storage = {}; // if not need -> remove
                                continue;
                            }
                        }
                    }

                    // for statement
                    if (token_table[idx] == "for") {
                        AST for_block(ForStmt, {}, {}, parsing_line);
                        std::pair<int, int> res = Parse(for_block, Token_table, idx+1, 1, token_storage);
                        head.AddChild(for_block);
                        
                        parsing_line = res.first;
                        idx = res.second;
                        prev_stmt = ForStmt;
                        token_storage = {}; // if not need -> remove
                        continue;
                    }
                    break;

                    if (token_table[idx] == "=") {
                        if (token_storage.size() == 1) { // assignment
                            AST assignment(Assignment, {Object("_", {}, {}, {"_", token_storage[0]})}, {}, parsing_line);
                            token_storage = {};

                            std::pair<int, int> res = Parse(assignment, Token_table, idx+1, 1, token_storage);
                            head.AddChild(assignment);

                            parsing_line = res.first;
                            idx = res.second;
                            prev_stmt = Assignment;
                            token_storage = {};
                            continue;
                        }
                        if (token_storage.size() == 2) { // declaration
                            AST var_del(VarDel, {Object("_", {}, {}, {"_", token_storage[0]}), Object("_", {}, {}, {"_", token_storage[1]})}, {}, parsing_line);
                            token_storage = {};

                            std::pair<int, int> res = Parse(var_del, Token_table, idx+1, 1, token_storage);
                            head.AddChild(var_del);

                            parsing_line = res.first;
                            idx = res.second;
                            prev_stmt = Assignment;
                            token_storage = {};
                            continue;
                        }
                    }

                    // const variable declaration
                    if (token_table[idx] == "const") { 
                        AST const_del(ConstDel, {}, {}, parsing_line);
                        std::pair<int, int> res = Parse(const_del, Token_table, idx+1, 1, token_storage);
                        head.AddChild(const_del);

                        parsing_line = res.first;
                        idx = res.second;
                        prev_stmt = ConstDel;
                        token_storage = {};
                        continue;
                    }

                    // break statement
                    if (token_table[idx] == "break") {
                        AST break_stmt(BreakStmt, {}, {}, parsing_line);
                        head.AddChild(break_stmt);
                    }

                    // continue statement
                    if (token_table[idx] == "continue") {
                        AST continue_stmt(ContinueStmt, {}, {}, parsing_line);
                        head.AddChild(continue_stmt);
                    }
                }

                // if-family statement
                case IfStmt: {
                    if (token_table[idx] == "?") {
                        if (arg_idx == 1) {
                            head.AddExpr(
                                ParseExpr(token_storage, parsing_line)
                            );
                            token_storage = {};
                            return Parse(head, Token_table, idx+1, 2, token_storage);
                        }
                    }
                    if (token_table[idx] == ";") {
                        if (arg_idx == 2) {
                            return {parsing_line, idx};
                        }
                    }
                    break;
                }
                
                case ElifStmt:
                case ElseStmt:
                    if (token_table[idx] == ";") {
                        return {parsing_line, idx};
                    }
                    break;
                

                // for statement
                case ForStmt: {
                    if (token_table[idx] == "in") {
                        if (arg_idx == 1 && token_storage.size() == 1) {
                            head.AddArg(Object("_", {}, {}, {"_", "C"}, 0, parsing_line));
                            head.AddArg(Object("_", {}, {}, {"_", token_storage[0]}, 0, parsing_line));
                            token_storage = {};
                            return Parse(head, Token_table, idx+1, 2, token_storage);
                        }
                    }
                    if (token_table[idx] == "~") { // todo: allow omission of <step>
                        if (arg_idx == 2 | arg_idx == 3) {
                            head.AddExpr(
                                ParseExpr(token_storage, parsing_line)
                            );
                            token_storage = {};
                            return Parse(head, Token_table, idx+1, arg_idx+1, token_storage);
                        }
                    }
                    if (token_table[idx] == "$") {
                        if (arg_idx == 1) {
                            head.AddArg(Object("_", {}, {}, {"_", "S"}, 0, parsing_line));
                            head.AddExpr(
                                ParseExpr(token_storage, parsing_line)
                            );
                            token_storage = {};
                            return Parse(head, Token_table, idx+1, 5, token_storage);
                        }
                        if (arg_idx == 4) {
                            head.AddExpr(
                                ParseExpr(token_storage, parsing_line)
                            );
                            token_storage = {};
                            return Parse(head, Token_table, idx+1, 5, token_storage);
                        }
                    }
                    if (token_table[idx] == ";") {
                        if (arg_idx == 5) {
                            return {parsing_line, idx};
                        }
                    }
                }
                
                // const variable declaration
                case ConstDel: {
                    if (token_table[idx] == "=") {
                        if (token_storage.size() == 2) { // declaration
                            head.AddArg(
                                Object("_", {}, {}, {"_", token_storage[0]})
                            );
                            head.AddArg(
                                Object("_", {}, {}, {"_", token_storage[1]})
                            );
                            token_storage = {};

                            return Parse(head, Token_table, idx+1, 2, token_storage);
                        }
                    }
                }

            }
            token_storage.push_back(token_table[idx]);
        }

        switch (head.GetStmt()) {
            case Main:
                AST expression(Expression, {}, {ParseExpr(token_table, parsing_line)}, parsing_line);
                head.AddChild(expression);
                token_storage = {};
                break;
        }

    }
}