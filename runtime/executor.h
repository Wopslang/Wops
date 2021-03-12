/*
 * runtime/executor.h
 * Wopslang Runtime Executor Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPSLANG_EXECUTOR_HEADER
#define WOPSLANG_EXECUTOR_HEADER

// Dependency
// C++: >= C++11
// use executor_test.cpp for test.

// 의존 헤더
#include "../error/errorhandler.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <stack>

// enum Type
// 변수의 타입을 명시하는 열거체
// 참고1: bool은 int와 저장 자료형은 같지만, 처리 시에는 if 0 => 0, else 1 을 따른다.
// 즉, bool은 1과 0만 가질 수 있는 것이다.
// TODO: bool 변수 대입 시에는 true false를 못 써요. 해결 부탁
enum Type {
    INT,
    STRING,
    BOOL,
    DOUBLE,
};

// struct Variable
// 변수가 기본적으로 가지고 있는 데이터 구조
struct Variable {
    std::string tag;  // 변수 이름
    std::string value;  // 변수 저장 값
    Type type;  // 자료형
};

// struct ReturnValue(called ReturnValue via typedef)
// 함수실행, Lvalue(변경 가능한 값) 조작할 때 값과 에러가 같이 나타날 경우의 구조
typedef struct ReturnValue {
    ErrorCode code;  // 실행 결과 코드
    std::string value;  // 결과값 (code != OK면 항상 "")
} ReturnValue;

// immutable operators[5]
// 모든 식을 구성할 수 있는 연산자들
// TODO: 다른 연산자를 추가해야 함. class Executor의 calc 함수와 calculate 함수 참조 바람.
const std::string operators[5] = {"+", "-", "*", "/", "%"};  // 참고2: string 연산: +만 가능, double 연산: %만 불가 

// immutable string special_token
// NOTE: 사용자가 이 토큰을 사용하여 강제적으로 에러를 생성할 수 있다.
const std::string special_token = "!#$%^&*(WOPSSPECIALTOKEN)";

// class Executor()
// 런타임 시행자. (parser/parser.cpp가 바이트코드를 main.cpp로 넘기면 main.cpp가 해석 후 이를 Executor에게 전달)]
// 
// NOTE: int는 long int형을, double은 long double을, string은 std::string을(Ref. <string>), bool은 int(Ref. 참고1)을 이용해 저장한다.
// 참고3: int와 double을 섞은 연산을 사용할 때에는 무조건 Double 연산으로 진행된다.
// 
// Executor Struct Tree
// Executor
//      - private
//          struct oper: 계산식을 구성하는 각 유닛 연산자들을 저장하는 용도
//          stack<long int> num: 사용되는 정수 연산 스택
//          stack<long double> numd: 사용되는 부동소수점 연산 스택
//          stack<oper> op: 사용되는 연산자 스택(가장 우선순위가 높은 Unit부터 pop된다)
//      - public
//          vector<struct Variable>: 변수 저장 캐쉬(메모리)
//          vector<string> tokens: 변수 이름과 같은 스트링토큰을 저장하는 벡터 (선언 많아지면 생기는 성능 저하 방지)
//          string ReplaceAll(string &str, const string& from, const string& to): str에서 from을 모두 to로 바꾸는 함수
//          void calcINT(): 정수 유닛 연산 시행자 함수
//          void calcDOUBLE(): 부동 소수점 유닛 연산 시행자 함수
//          string calculate(Type type, string value): 자료형이 type인 value가 문자열로 들어오면 연산결과를 문자열로 변환하는 함수
//          ReturnValue modifyString(Type type, string value): 자료형이 type인 value가 문자열로 들어오면 시행 및 알맞게 면화된 값을 
//                                                             ReturnValue에 담아 반환하는 함수
//          ErrorCode createVar(Type type, string tag, string value=""): 자료형이 type인 변수(이름: tag, 값: value)를 선언하고
//                                                                       실행 결과를 반환하는 함수
//          ErrorCode modifyVar(string tag, string value): 이름이 tag인 변수의 값을 value로 바꾼 후 실행결과를 반환하는 함수
//          ReturnValue callfunc(string name, vector<string> argv): 함수 이름이 name인 함수를 argv를 매개변수로 하여금 실행하는 함수

class Executor {
    private:
        // struct Executor::oper
        // 계산식을 구성하는 각 유닛 연산자들을 저장하는 Structure
        struct oper {
            int p;
            std::string o;
        };

        // 연산 함수에 필요한 스택들
        std::stack<long int> num;
        std::stack<long double> numd;
        std::stack<oper> op;

    public:
        // Class Executor 생성자
        Executor () {
            num = {};
            op = {};
        }

        // vector<struct Variable> memories: 변수 저장 캐쉬(벡터 사용)
        std::vector<struct Variable> memories;

        // vector<string> tokens: memories의 tag만 따로 저장한 벡터
        std::vector<std::string> tokens;

        // string calculate(Type type, string value): 자료형이 type인 value가 문자열로 들어오면 연산결과를 문자열로 변환하는 함수
        std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to){
            size_t start_pos = 0; 
            
            // 더 이상 from이 발견되지 않을 때까지 반복
            while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length();
            }
            return str;
        }
        
        // void calcINT(): 정수 유닛 연산 시행자 함수
        void calcINT() {
            // 가장 먼저 시행될 연산을 불러오는 단계
            long int a, b, result;
            b = num.top();
            num.pop();
            a = num.top();
            num.pop();
            std::string oper = op.top().o;
            op.pop();

            // 계산
            if (oper == "*")
                result =  a * b;
            else if (oper == "/") {
                if (b == 0) result = 0;
                else result = a / b;
            }
            else if (oper == "%")
                result = a % b;
            else if (oper == "+")
                result = a + b;
            else if (oper == "-")
                result = a - b;
            
            // 결과 스택에 넣는 과정
            num.push(result);
        }
        
        // void calcDOUBLE(): 부동 소수점 유닛 연산 시행자 함수
        // 코드 설명은 calcINT()를 참조해라.
        void calcDOUBLE() {
            long double a, b, result;
            b = numd.top();
            numd.pop();
            a = numd.top();
            numd.pop();
            std::string oper = op.top().o;
            op.pop();
         
            if (oper == "*")
                result =  a * b;
            else if (oper == "/") {
                if (b == 0) result = 0;
                else result = a / b;
            }
            else if (oper == "%")  // 연산 시 0이 나오게 했다. TODO: 에러를 쓰는 방안 검토
                result = 0;
            else if (oper == "+")
                result = a + b;
            else if (oper == "-")
                result = a - b;
            numd.push(result);
        }

        // string calculate(Type type, string value): 자료형이 type인 value가 문자열로 들어오면 연산결과를 문자열로 변환하는 함수
        // 타입 별로 시행하는 연산이 다르다. 참고 바람
        // NOTE: DivideByZero와 같은 에러가 발생할 수 있는 케이스에 대해서는 결과가 0이 되도록 했다.
        std::string calculate(Type type, std::string value) {
            std::string input = value;
            std::stringstream ss(input);  // 문자열을 공백 간격으로 나눈다

            // 계산 문자열 타입이라면
            // "hello" + "world"와 같은 연산밖에 할 수 없다. ! 괄호 지원하지 않는다.
            if (type == STRING) {
                // 만약 value에 "이 포함된다면 (TODO: 문법 검사기 만들 때 " 짝 확인 잊지 말아야 함)
                if (find(input.begin(), input.end(), '\"') != input.end()) {
                    std::string ret = ""; std::string cache = "";
                    bool is_string = false;
                    for (int i = 0; i < input.length(); i++) {
                        if (input[i] == '\"') is_string = !is_string;
                        else if (!is_string && input[i] == '+') {
                            ret += cache;
                            cache = "";
                        }
                        else if (is_string) cache += input[i];
                    }
                    if (cache != "") ret += cache;  // +가 없는 식(Ex. "hello world") 경우
                    ret = "\"" + ret + "\"";
                    return ret;
                }

                // 문자열이 아닌데 문자열으로 보내는 경우는 callfunc 밖에 없다.
                // 즉 이 계산은 문자열이 아니라고 에러를 보내줘야 한다.
                return special_token + "NOT_MATCHING_TYPE";
            }

            // 숫자(int, double, bool) 연산
            std::string tok;
            while (ss >> tok) {
                // 괄호 체크
                if (tok == "(") {
                    op.push({0, tok});
                }
                else if (tok == ")") {
                    while (op.top().o != "(")
                        switch (type) {
                            case INT:
                                calcINT();
                                break;
                            case BOOL:
                                calcINT();
                                break;
                            case DOUBLE:
                                calcDOUBLE();
                                break;
                            case STRING:
                                // 여기로 올 일 없다.
                                break;
                        }
                    op.pop();
                }

                // 연산 우선순위 이용해 스택 차례대로 쌓는다.
                else if (tok == "*" || tok == "/" || tok == "%" || tok == "+" || tok == "-") {
                    int prior;
                    if (tok == "*")
                        prior = 4;
                    else if (tok == "/")
                        prior = 4;
                    else if (tok == "%")
                        prior = 4;
                    else if (tok == "+")
                        prior = 3;
                    else if (tok == "-")
                        prior = 3;
                             
                    while (!op.empty() && prior <= op.top().p)
                        switch (type) {
                            case INT:
                                calcINT();
                                break;
                            case BOOL:
                                calcINT();
                                break;
                            case DOUBLE:
                                calcDOUBLE();
                                break;
                            case STRING:
                                // No percentage to flow here :D
                                break;
                        }
                    op.push({prior, tok});
                }
                else
                    // 여기서 에러가 나도 괜찮다. 우리는 try문을 가지고 있으니까!
                    switch (type) {
                        case INT:
                            try {
                                num.push(stol(tok));
                            } catch (std::runtime_error e) {
                                return special_token + "NOT_MATCHING_TYPE";
                            }
                            break;
                        case BOOL:
                            try {
                                num.push(stol(tok));
                            } catch (std::runtime_error e) {
                                return special_token + "NOT_MATCHING_TYPE";
                            }
                            break;
                        case DOUBLE:
                            try {
                                numd.push(stold(tok));
                            } catch (std::runtime_error e) {
                                return special_token + "NOT_MATCHING_TYPE";
                            }
                            break;
                        case STRING:
                            // 역시 여기로는 올 수 없다.
                            break;
                    }
            }
            while (!op.empty()) {
                switch (type) {
                    case INT:
                        calcINT();
                        break;
                    case BOOL:
                        calcINT();
                        break;
                    case DOUBLE:
                        calcDOUBLE();
                        break;
                    case STRING:
                        // No percentage to flow here :D
                        break;
                }
            }

            switch (type) {
                // 문자열으로 바꾸는 데 에러가 발생할 가능성이 없다.
                case INT:
                    return std::to_string(num.top());
                case BOOL:
                    return std::to_string(num.top());
                case DOUBLE:
                    return std::to_string(numd.top());
                case STRING:
                    // No percentage to flow here :D
                    return value;
            }
        }
        
        // ReturnValue modifyString(Type type, string value): 자료형이 type인 value가 문자열로 들어오면 시행 및 알맞게 면화된 값을 
        //                                                    ReturnValue에 담아 반환하는 함수
        // TODO: 이 함수는 띄어쓰기가 없는 것을 전제로 한다. ("hello" + " world": x, "hello"+" world": o)
        ReturnValue modifyString(Type type, std::string value) {
            int last_operator_idx = -1;  // 이전 연산자 인덱스
            std::string last_token = "";  // 연산자 전 나온 토큰
            bool is_nowchecking_string = false;  // 문자열 내에서는 모든 수정을 멈춰야 하므로 이를 토글해주는 문자열

            for (int i = 0; i < value.length(); i++) {
                std::string tmp_tocmpr = value.substr(i, 1);  // char을 string으로 미리 바꿈(std::find 사용 위해서)
                
                // 문자열 토글
                if (value[i] == '\"') {
                    if (is_nowchecking_string) is_nowchecking_string = false;
                    else is_nowchecking_string = true;
                    continue;
                }

                // 문자열이 아니면서 숫자도 아니고, 연산자도 아니고, 괄호나 소수점도 아닌, 즉 변수 또는 함수 실행일 때 last_token에 이번 문자 저장
                if (!is_nowchecking_string && !isdigit(value[i]) && find(operators, operators+5, tmp_tocmpr) == operators+5 && (value[i] != '(' && value[i] != ')') && value[i] != '.') {
                    last_token += value[i];
                }

                // 이번 문자가 연산자일때
                else if (find(operators, operators+5, tmp_tocmpr) != operators+5) {
                    if (last_token == "" || value[i-1] == '\"') {  // 문자열 검색 후 바로 다음에 오는 연산자일때(Ex. "hello"+"world")
                        last_operator_idx = i;
                    } else {
                        // last_token이 ""가 아니면 무조건 변수
                        int token_idx = -1;
                        for (int j = 0; j < tokens.size(); j++) {
                            if (last_token == tokens[j]) {
                                token_idx = j;
                                break;
                            }
                        }

                        if (token_idx == -1) {
                            ReturnValue ret {NOT_EXIST_VARIABLE, ""};
                            return ret;
                        }
                        value.replace(value.begin() + last_operator_idx + 1, value.begin() + i, memories[token_idx].value);
                        i = last_operator_idx + memories[token_idx].value.length();  // 문자열이 수정되었으므로 인덱스도 변해야 함.
                        last_operator_idx = i;
                        last_token = "";
                    }
                }

                // 이외의 경우: 숫자 연산
                if (!is_nowchecking_string && find(operators, operators+5, tmp_tocmpr) != operators+5 && value[i] != '.') {
                    last_operator_idx = i;
                }
                
                // 이외의 경우: 함수 실행
                else if (value[i] == '(' && i > 0 && last_operator_idx != i - 1) {
                    int idx;
                    for (idx = i+1; idx < value.length(); idx++) {
                        if (value[idx] == ')') break;
                    }
                    std::vector<std::string> argv;
                    std::string cache_string;

                    for (int j = i+1; j < idx; j++) {
                        if (value[j] == ',') {
                            argv.push_back(cache_string);
                            cache_string = "";
                        } else {
                            cache_string += value[j];
                        }
                    }
                    
                    argv.push_back(cache_string);
                    ReturnValue callResult = callfunc(
                            value.substr(last_operator_idx+1, i-(last_operator_idx+1)),
                            argv
                    );

                    if (callResult.code != OK) {
                        ReturnValue ret {callResult.code, ""};
                        return ret;
                    }
                    value.replace(value.begin() + last_operator_idx + 1, value.begin() + idx + 1, callResult.value);
                    i += callResult.value.length() - (i - last_operator_idx);
                    last_operator_idx = i;
                    last_token = "";
                }
            }

            // 식이 변수 하나밖에 없을 때 (ex. boo = foo)
            if (last_token != "") {
                int token_idx = -1;
                for (int i = 0; i < tokens.size(); i++) {
                    if (last_token == tokens[i]) {
                        token_idx = i;
                        break;
                    }
                }

                if (token_idx == -1) {
                    ReturnValue ret {NOT_EXIST_VARIABLE, ""};
                    return ret;
                }
                value.replace(value.begin(), value.end(), memories[token_idx].value);
                last_token = "";
            }


            // calculate 함수에 값을 넣기 위해서 연산자 양옆에 간격을 만드는 작업
            for (int i = 0; i < 5; i++) {
                value = ReplaceAll(value, operators[i], " " + operators[i] + " ");
            }
            value = ReplaceAll(value, "(", " ( ");
            value = ReplaceAll(value, ")", " ) ");

            ReturnValue ret = {OK, calculate(type, value)};
            return ret;
        }

        // ErrorCode createVar(Type type, string tag, string value=""): 자료형이 type인 변수(이름: tag, 값: value)를 선언하고
        //                                                              실행 결과를 반환하는 함수
        ErrorCode createVar(Type type, std::string tag, std::string value="") {
            // 이미 변수가 존재하는 지 확인
            int idx = -1;
            for (int i = 0; i < memories.size(); i++) {
                if (memories[i].tag == tag) {
                    idx = i; break;
                }
            }

            if (idx != -1) return ALREADY_EXIST;

            // modify
            ReturnValue modified = modifyString(type, value);
            if (modified.code != OK) return modified.code;

            // 자료형에 따른 처리
            if (value != "") {
                switch (type) {
                    case INT:
                        for (int i = 0; i < modified.value.length(); i++) {
                            if (!isdigit(modified.value[i])) {
                                return NOT_MATCHING_TYPE;
                            }
                        }
                        break;

                    case STRING:
                        if (!(modified.value[0] == '\"' && modified.value[modified.value.length()-1] == '\"')) {
                            return NOT_MATCHING_TYPE;
                        }
                        break;

                    case BOOL:
                        for (int i = 0; i < modified.value.length(); i++) {
                            if (!isdigit(modified.value[i])) {
                                return NOT_MATCHING_TYPE;
                            }
                        }
                        modified.value = (modified.value == "0" ? "0" : "1");
                        break;

                    case DOUBLE:
                        std::string ret = ""; bool already_have_dot = false;
                        for (int i = 0; i < modified.value.length(); i++) {
                            if (modified.value[i] == '.') {
                                if (i == 0 || i == modified.value.length() - 1 || already_have_dot) {
                                    return STRANGE_DOUBLE;
                                }
                                if (isdigit(modified.value[i+1]))
                                already_have_dot = true;
                                ret += modified.value[i];
                            }
                            else if (!isdigit(modified.value[i])) {
                                return NOT_MATCHING_TYPE;
                            }
                            else {
                                ret += modified.value[i];
                            }
                        }
                        modified.value = ret;
                }
            }

            // 캐시에 저장
            struct Variable nvariable {tag, modified.value, type};
            memories.push_back(nvariable);
            tokens.push_back(tag);
            return OK;
        }

        // ErrorCode modifyVar(string tag, string value): 이름이 tag인 변수의 값을 value로 바꾼 후 실행결과를 반환하는 함수
        ErrorCode modifyVar(std::string tag, std::string value) {
            int idx = -1;
            for (int i = 0; i < tokens.size(); i++) {
                if (tokens[i] == tag) {
                    idx = i; break;
                }
            }
            if (idx == -1) return NO_VARIABLE_TO_MODIFY;

            ReturnValue modified = modifyString(memories[idx].type, value);
            if (modified.code != OK) return modified.code;

            // 자료형 체크
            switch (memories[idx].type) {
                case INT:
                    for (int i = 0; i < modified.value.length(); i++) {
                        if (!isdigit(modified.value[i])) {
                            return NOT_MATCHING_TYPE;
                        }
                    }
                    break;

                case STRING:
                    if (!(modified.value[0] == '\"' && modified.value[modified.value.length()-1] == '\"')) {
                        return NOT_MATCHING_TYPE;
                    }
                    break;

                case BOOL:
                    for (int i = 0; i < modified.value.length(); i++) {
                        if (!isdigit(modified.value[i])) {
                            return NOT_MATCHING_TYPE;
                        }
                    }
                    modified.value = (modified.value == "0" ? "0" : "1");
                    break;

                case DOUBLE:
                    std::string ret = ""; bool already_have_dot = false;
                    for (int i = 0; i < modified.value.length(); i++) {
                        if (modified.value[i] == '.') {
                            if (i == 0 || i == modified.value.length() - 1 || already_have_dot) {
                                return STRANGE_DOUBLE;
                            }
                            already_have_dot = true;
                        }
                        else if (!isdigit(modified.value[i])) {
                            return NOT_MATCHING_TYPE;
                        }
                        else {
                            ret += modified.value[i];
                        }
                    }
                    modified.value = ret;
            }
            memories[idx].value = modified.value;
            return OK;
        }

        // ReturnValue callfunc(string name, vector<string> argv): 함수 이름이 name인 함수를 argv를 매개변수로 하여금 실행하는 함수
        ReturnValue callfunc(std::string name, std::vector<std::string> argv) {
            // TODO: 나중에 함수를 자유롭게 정할 수 있게 모듈 생성 혹은 데이터 구조 생성
            if (name == "in") {  // func<str> in()
                std::string tmp; std::cin >> tmp;
                ReturnValue ret {OK, "\"" + tmp + "\""};
                return ret;
            }
            if (name == "out") {  // func<> out(string argv, ...) (...는 작동하지 않음)
                for (int i = 0; i < argv.size(); i++) {
                    ReturnValue argv_now;
                    switch (STRING) {
                        case STRING:
                            argv_now = modifyString(STRING, argv[i]);
                            if (argv_now.value != special_token + "NOT_MATCHING_TYPE") break;
                        case BOOL:  // same with int
                        case INT:
                            argv_now = modifyString(INT, argv[i]);
                            if (argv_now.value != special_token + "NOT_MATCHING_TYPE") break;
                        case DOUBLE:
                            argv_now = modifyString(DOUBLE, argv[i]);
                            if (argv_now.value != special_token + "NOT_MATCHING_TYPE") break;
                        default:
                            return {NOT_MATCHING_TYPE, ""};
                    }
                    if (argv_now.code != OK) {
                        ReturnValue ret {argv_now.code, ""};
                        return ret;
                    }
                    std::string sentence = argv_now.value;
                    sentence.erase(remove(sentence.begin(), sentence.end(), '\"'), sentence.end());
                    std::cout << sentence;
                }
                ReturnValue ret {OK, ""};
                return ret;
            }

            ReturnValue ret {NOT_EXIST_FUNCTION, ""};
            return ret;
        }
};
#endif
