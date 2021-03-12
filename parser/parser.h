/*
 * parser/parser.h
 * Wopslang Runtime Error Handler Headerfile
 *
 * 2021, Wops Team
 * */

// dependency
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <stack>

using namespace std;

// 타입들과 빌트인 펑션(Ref. runtime/executor.h)
const string types[4] = {"int", "double", "bool", "string"};
const string builtin_func[2] = {"out", "in"};

// enum expression
// 어떤 유형의 코드인지 명시해주는 열거형
enum expression {
    IF,
    ELIF,
    ELSE,
    FOR,
};

// class Parser
// 파서 클래스
class Parser {
    private:
        // std::vector<std::string> split(std::string input, char delimiter)
        // 문자열을 공백 간격으로 나누는 과정
        // TODO: stringstream으로 변환
        static vector<string> split(const string& input, char delimiter);
    public:
        // std::string Parse(std::string codepath)
        // 파일 경로 codepath가 가리키는 파일을 파싱함
        static string Parse(const string& codepath);
};

vector<string> Parser::split(const string& input, char delimiter) {
    vector<string> answer;
    stringstream ss(input);
    string temp;
    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }
    return answer;
}

string Parser::Parse(const string& codepath) {
    stack<expression> bracePair;
    ifstream file(codepath);

    string code;
    string bytecode;

    while (file.peek() != EOF) {
        getline(file, code);

        vector<string> tokens = split(code, ' ');

        // call builtin_func
        if (find(builtin_func, builtin_func+2, tokens[0]) != builtin_func+2 && tokens[1] == "(") {
            string k;
            int a = 0, b = 0;
            for (int i = 0; i < tokens.size(); i++) {
                if(tokens[i] == "(")
                    a = i;
                else if(tokens[i] == ")")
                    b = i;
            }
            for (int i = a + 1; i < b; i++)
                k += " " + tokens[i];
            if (k.empty()) {
                bytecode += "callfunc " + tokens[0] + "";
            } else {
                bytecode += "callfunc " + tokens[0] + (" =>" + k);
            }
        }

        // declare variable
        if (find(types, types+4, tokens[0]) != types+4) {
            // not initialize
            if (tokens.size() == 2) {
                bytecode += "declare " + tokens[1] + "(" + tokens[0] + ")";
            } else {
                string ret = " " + tokens[3];
                for (int i = 4; i < tokens.size(); i++) {
                    ret += " " + tokens[i];
                }
                bytecode += "declare " + tokens[1] + "(" + tokens[0] + ")" + " =>" + ret;
            }
        }

            // declare constant
        else if (tokens[0] == "const") {
            string ret = " "  + tokens[4];
            for (int i = 5; i < tokens.size(); i++) {
                ret += " " + tokens[i];
            }
            bytecode += "declconst " + tokens[2] + "(" + tokens[1] + ")" + " =>" + ret;
        }

            // modify variable
        else if (tokens[1] == "=") {
            string ret = " " + tokens[2];
            for (int i = 3; i < tokens.size(); i++) {
                ret += " " + tokens[i];
            }

            bytecode += "modify " + tokens[0] + " =>" + ret;
        }

            // if
        else if (tokens[0] == "if") {
            string k;
            int a = 0, b = 0;
            for(int i = 0; i < tokens.size(); i++) {
                if(tokens[i] == "(")
                    a = i;
                else if(tokens[i] == ")")
                    b = i;
            }
            for (int i = a + 1; i < b; i++)
                k += " " + tokens[i];
            bytecode += "ifstart" + k;
            bracePair.push(IF);
        }

            // else if
        else if (tokens[0] == "elif") {
            string k;
            int a = 0, b = 0;
            for(int i = 0; i < tokens.size(); i++) {
                if(tokens[i] == "(")
                    a = i;
                else if(tokens[i] == ")")
                    b = i;
            }
            for (int i = a + 1; i < b; i++)
                k += " " + tokens[i];
            bytecode += "elifstart" + k;
            bracePair.push(ELIF);
        }

            // else
        else if (tokens[0] == "else") {
            bytecode += "elsestart";
            bracePair.push(ELSE);
        }

            // for
        else if (tokens[0] == "for") {
            // for range or list
            if (tokens[2] == "in") {
                if (tokens[3] == "range") {  // for range
                    string ret;
                    int a = 0, b = 0;
                    for (int i = 0; i < tokens.size(); i++) {
                        if(tokens[i] == "(")
                            a = i;
                        else if(tokens[i] == ")")
                            b = i;
                    }

                    for (int i = a+1; i < b; i++) {
                        if (tokens[i] == ",") continue;
                        ret += " " + tokens[i];
                    }
                    bytecode += "forstart " + tokens[1] + " =>" + ret;
                } else {  // for in list
                    string ret;
                    for (int i = 3; i < tokens.size(); i++) {
                        ret += " " + tokens[i];
                    }

                    bytecode += "forstart " + tokens[1] + " => " + ret;
                }
            } else {  // while
                string k;
                int a = 0, b = 0;

                for (int i = 0; i < tokens.size(); i++) {
                    if(tokens[i] == "(")
                        a = i;
                    else if(tokens[i] == ")")
                        b = i;
                }
                for (int i = a + 1; i < b; i++) {
                    if (tokens[i] == ",") continue;
                    k += " " + tokens[i];
                }
                bytecode += "forstart while" + k;
            }
            bracePair.push(FOR);
        }

        else if(tokens[0] == "//") {
            bytecode += "comment";
        }

        else if (tokens[0] == "}") {
            expression tmp = bracePair.top();
            switch (tmp) {
                case IF:
                    bytecode += "ifend";
                    break;
                case ELIF:
                    bytecode += "elifend";
                    break;
                case ELSE:
                    bytecode += "elseend";
                    break;
                case FOR:
                    bytecode += "forend";
                    break;
            }

            bracePair.pop();
        }
        bytecode += "\n";
    }
    return bytecode;
}
