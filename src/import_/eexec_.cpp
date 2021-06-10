/*
 * src/import_/eexec_.cpp
 * Wopslang External Function Executor Sourcefile
 *
 * 2021, Wops Team
 * */

#include <dlfcn.h>
#include <iostream>
#include "../type/variable.h"

VariableWithCode EExecFunc(std::string func, Variable argv) {
    // VARIABLE
    Variable null = Variable("_", "", INT);

    void* handle = dlopen("./dll/library.so", RTLD_LAZY);

    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return {null, ERROR};
    }

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

// unit test
int main() {
    std::string funcname = "out";
    Variable s = Variable("_", "Wopslang!", STRING);
    VariableWithCode result_code = EExecFunc(funcname, s);
}