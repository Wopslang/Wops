/*
 * src/type/variable.h
 * Wopslang Variable Template Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_VARIABLE_H
#define WOPS_VARIABLE_H
#include <string>
#include "../error/signal.h"

// 추후 변경시 Refactor을 줄이기 위한 Alias
using Int=signed int;
using Double=double;
using String=std::string;
using Bool=bool;

// enum TYPE {INT, DOUBLE, STRING, BOOL}
// Wopslang 데이터타입 열거형
enum TYPE {
    INT,
    DOUBLE,
    STRING,
    BOOL,
};

/** 
 * class Variable
 * 변수 컨테이너 클래스: 변수는 물론 모든 단일 값을 저장한다.
 * 
 * Structure
 * 
 * Variable
 *   - value
 *   - token (=variable name)
 *   - type
*/
class Variable {
    private:
    public:
    Variable(std::string varname, std::string val, TYPE t) { // constructor
        assert(varname != ""); 
        value = val;
        token = varname;
        _t = t;
    }

    Err Substitute(std::string newval) { // substitute
        // :TODO add type checker
        value = newval;
        return OK;
    }

    std::string GetValue() { // extract value
        return value;
    }

    std::string value;
    std::string token;
    TYPE _t;
};

// struct VariableWithCode
// Variable 컨테이너와 오류 메시지를 동시에 지니고 있는 구조체. 변환값에 자주 사용
struct VariableWithCode {
    Variable var;
    Err error;
};

#endif