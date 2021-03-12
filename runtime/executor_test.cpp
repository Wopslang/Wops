/*
 * runtime/executor_test.cpp
 * Wopslang Runtime Executor Testfile
 *
 * 2021, Wops Team
 * */

// test target
#include "executor.h"

int main() {
    Executor executor {};
    
    // 1. callfunc, Hello, Wops!
    std::vector<std::string> argv {"\"Hello, Wops! \"", "\"I'm RedoC\n\""};
    ReturnValue code = executor.callfunc("out", argv);

    // 2. callfunc, CAT program
    argv = {""};
    code = executor.callfunc("in", argv);
    argv = {code.value, "\"\n\""};
    code = executor.callfunc("out", argv);

    // 3. declare variable
    ErrorCode code2 = executor.createVar(STRING, "name", "\"Wops\"");
    if (code2 != OK) return 1;
    argv = {"name", "\"\n\""};
    code = executor.callfunc("out", argv);

    // 4. modify variable
    code2 = executor.modifyVar("name", R"("Wops"+"Lover")");
    if (code2 != OK) return 1;
    argv = {"name", "\"\n\""};
    code = executor.callfunc("out", argv);

    // 5. modify variable with func
    code2 = executor.modifyVar("name", "in()+in()");
    if (code2 != OK) return 1; 
    argv = {"name", "\"\n\""};
    code = executor.callfunc("out", argv);

    // 6. callfunc in func (upgrade from 2)
    argv = {"in()+\"\n\""};
    code = executor.callfunc("out", argv);

    // 7. modify variable with variable
    code2 = executor.createVar(STRING, "name2", "name");
    if (code2 != OK) return 1; 
    argv = {"name2 + \"\n\""};
    code = executor.callfunc("out", argv);

    // 7. number variable 
    code2 = executor.createVar(INT, "test", "(35*50)%10-2+5");
    if (code2 != OK) return 1; 
    argv = {"test", "\"\n\""};
    code = executor.callfunc("out", argv);

    // 8. number(double) variable 
    code2 = executor.createVar(DOUBLE, "boofoo", "3.2+5.8");
    if (code2 != OK) return 1;
    argv = {"boofoo"};
    code = executor.callfunc("out", argv);
    return 0;
}
