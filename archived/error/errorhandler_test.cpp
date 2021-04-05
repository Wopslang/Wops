/*
 * error/errorhandler_test.cpp
 * Wopslang Runtime Error Handler Testfile
 *
 * 2021, Wops Team
 * */

// test target
#include "errorhandler.h"

// test dependency
#include <iostream>

int main() {
    try {
        ErrorHandler::ThrowError(NOT_EXIST_VARIABLE);
    } catch (const char* e) {
        std::cout << "Error (" << e << ") Throwed" << std::endl;
        return 0;
    }
    return 1;
}
