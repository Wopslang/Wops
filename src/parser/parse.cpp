/*
 * src/parser/parse.cpp
 * Wopslang Parsing Sourcefile
 *
 * 2021, Wops Team
 *
 * */

#include "parse.h"

std::vector<char> oprs{
    '+', '-', '*', '/', '%', '=', '>', '<', '!', '&', '|', '(', ')', '[', ']', ':', ';', ','
};

std::vector<String> operators{
    "+", "-", "*", "/", "%", "=", "==", "!=", ">", "<", ">=", "<=", "!", "&&", "||"
};

std::vector<std::pair<String, String>> runes{
    {"\a", "a"}, {"\b", "b"}, {"\f", "f"}, {"\n", "n"}, {"\r", "r"}, {"\t", "t"}, {"\v", "v"}, {"\\", "\\"}, {"\'", "'"}, {"\"", "\""}
};

Expr ParseExpr(std::vector<String> tokens, int parsing_line) {
    Expr head({0,0,0}, Variable("_", "", OPERATOR), parsing_line);
    return head;
}

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

void Parse(AST& head, std::vector<String> codes) {
    int parsing_line = head.codeline;
    for (String code: codes) {
        parsing_line++;
        std::vector<String> token_table = GetTokenTable(code);
    }
}

int main() {
    for (String e: GetTokenTable("ab=="))
        std::cout << e << "\n";

    return 0;
}