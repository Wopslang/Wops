/*
 * parser/parser_test.cpp
 * Wopslang Runtime Parser Testfile
 *
 * 2021, Wops Team
 * */

// target dependency
#include "parser.h"

// test dependency
#include <iostream>

int main() {
    Parser parser;
    string ret = parser.Parse("parser/testfile.wops");
    cout << ret;
    return 0;
}