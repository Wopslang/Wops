/*
 * error/errorhandler.h
 * Wopslang Runtime Error Handler Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPSLANG_ERRORHANDLER_HEADER
#define WOPSLANG_ERRORHANDLER_HEADER

// dependency
#include <stdexcept>
#include <string>

// enum ErrorCode
// 처리 시 발생하는 결과 코드(OK, 나머지는 에러 코드)
enum ErrorCode {
    OK,
    NO_VARIABLE_TO_MODIFY,  // 선언되지 않은 변수를 변경하려 할 때
    ALREADY_EXIST,  // 이미 존재하는 변수를 재선언하려고 할 때
    NOT_EXIST_FUNCTION,  // 존재하지 않는 함수를 호출할 때
    NOT_EXIST_VARIABLE,  // 존재하지 않는 변수를 이용할 때
    NOT_FIT_ARGV,  // 선언된 함수와 맞지 않는 매개변수를 이용할 때
    NOT_MATCHING_TYPE,  // 맞지 않는 자료형
    STRANGE_DOUBLE,  // 문법, 또는 형태가 이상한 부동소수점을 이용할 때(3. , .12, 3.2.1)
};

// class ErrorHandler
// 단순히 에러를 핸들링한다. 
class ErrorHandler {
    public:
        static void ThrowError(ErrorCode e) {
            switch (e) {
                case NO_VARIABLE_TO_MODIFY:
                    throw "RUNTIME ERROR: tried to modify not exist variable.";
                
                case ALREADY_EXIST:
                    throw "RUNTIME ERROR: tried to re-declare exist variable.";
                
                case NOT_EXIST_FUNCTION:
                    throw "RUNTIME ERROR: tried to use not exist function.";
                
                case NOT_EXIST_VARIABLE:
                    throw "RUNTIME ERROR: tried to use not exist variable.";
            
                case NOT_FIT_ARGV:
                    throw "RUNTIME ERROR: used not fit arguments in function call";
                
                case NOT_MATCHING_TYPE:
                    throw "RUNTIME ERROR: used not matching type";
                
                case STRANGE_DOUBLE:
                    throw "RUNTIME-SYNTAX ERROR: used strange double";

                case OK:
                    break;
            }
        }
};

#endif