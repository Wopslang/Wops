/*
 * src/type/array.h
 * Wopslang Array Template Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_ARRAY_H
#define WOPS_ARRAY_H
#include "variable.h"
#include "../error/signal.h"
#include <vector>
#include <cstdarg>

// class Array
// 다변수가 할당, 저장 등 사용되는 경우 이용할 임시 클래스
class Array {
    private:
    public:
    std::vector<Variable> container;

    Array() {
        container.clear();
    }

    Array(Variable o) {
        container.push_back(o);
    }

    Array(std::vector<Variable> o) {
        container.reserve(o.size());
        container = o;
    }
};

// struct ArrayWithCode 
// Array와 오류 메시지를 동시에 지니고 있는 구조체. 변환값에 자주 사용
struct ArrayWithCode {
    Array var;
    Err error;
};

#endif