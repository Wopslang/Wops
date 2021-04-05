/*
 * runtime/variable_structure.h
 * Wopslang Variable Structure Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_VARIABLE_STRUCTURE_H
#define WOPS_VARIABLE_STRUCTURE_H

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

#endif //WOPS_VARIABLE_STRUCTURE_H
