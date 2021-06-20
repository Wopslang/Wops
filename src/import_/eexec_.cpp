/*
 * src/import_/eexec_.cpp
 * Wopslang External Function Executor Sourcefile
 *
 * 2021, Wops Team
 * */

#include <dlfcn.h> // to load dll
#include <iostream>
#include "../type/variable.h"

// VariableWithCode EExecFunc(std::string func, Variable argv)
// Calls a function with argv as a parameter and func as a name.
VariableWithCode EExecFunc(std::string func, Variable argv) {
    // EMPTY VARIABLE
    Variable null = Variable("_", "", INT);

    void* handle = dlopen("./dll/library.so", RTLD_LAZY);

    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return {null, ERROR};
    }

    // function pointer
    typedef VariableWithCode (*t)(Variable);

    // reset errors
    dlerror();

    std::string exfunc(func);
    t fptr = (t) dlsym(handle, exfunc.c_str());
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol '" << func << "': " << dlsym_error <<
            '\n';
        dlclose(handle);
        return {null, ERROR};
    }

    VariableWithCode ret = fptr(argv);
    dlclose(handle);
    return ret;
}

// unit test: out(in())
int main() {
    Variable s = Variable("_", "", STRING);
    VariableWithCode inp = EExecFunc("in", s);
    VariableWithCode _ = EExecFunc("out", inp.var);
}
