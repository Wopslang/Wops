/*
 * lib/library,cpp
 * Wopslang Library DLL Sourcefile
 *
 * 2021, Wops Team
 * */

#include <iostream>
#include <string>
#include "../src/type/variable.h"

// Standard I/O
// out(a), no type limit
extern "C" VariableWithCode out(Variable s) { 
    // VARIABLE
    Variable null = Variable("_", "", INT);
    
    std::cout << s.GetValue();
    return {null, OK};
}
