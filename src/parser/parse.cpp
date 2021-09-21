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
    '+', '-', '*', '/', '%', '=', '>', '<', '!', '&', '|', '(', ')', '[', ']', '{', '}'
};

AST Parse(std::vector<std::string> codes) {
    AST main(Main, {}, {});
    for (int idx = 0; idx < codes.size(); idx++) {
        std::string code = codes[idx];

        size_t lwhitespace = code.find_first_not_of(" \n\r\t\f\v");
        code = (lwhitespace == std::string::npos) ? "" : code.substr(lwhitespace);

        size_t rwhitespace = code.find_last_not_of(" \n\r\t\f\v");
        code = (rwhitespace == std::string::npos) ? "" : code.substr(0, rwhitespace+1);

        std::vector<std::string> tokens;
        bool isParsingString = 0;
        std::string cache = "";

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
        
        // unit test
        if (cache != "") tokens.push_back(cache);
        for (std::string e: tokens) std::cout << e << ",";
        std::cout << "\n";
    }
    return main;
}

int main() {
    Parse({"string a = \"Hello, \" + in()"});
}
