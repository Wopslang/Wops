/*
 * src/import_/eexec_.cpp
 * Wopslang External Function Executor Sourcefile
 *
 * 2021, Wops Team
 * */

#include <dlfcn.h> // to load dll
#include <iostream>
#include "../type/variable.h"
#include "../type/array.h"

// ArrayWithCode EExecFunc(std::string func, Array argv)
// Calls a function with argv as a parameter and func as a name.
ArrayWithCode EExecFunc(std::string func, Array argv) {
    // EMPTY VARIABLE
    Array null(Variable("_", "", INT));

    void* handle = dlopen("./dll/library.so", RTLD_LAZY);

    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return {null, ERROR};
    }

    // function pointer
    typedef ArrayWithCode (*t)(Array);

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

    ArrayWithCode ret = fptr(argv);
    dlclose(handle);
    return ret;
}

// unit test: out(toint(in())+toint(in()))
// YOU SHOULD INPUT ONLY INTEGER!
int main() {
    ArrayWithCode inp = EExecFunc("in", Array(Variable("_", "", STRING)));
    ArrayWithCode inp2 = EExecFunc("in", Array(Variable("_", "", STRING)));
    inp = EExecFunc(
        "toint",
        inp.var
    );
    inp2 = EExecFunc(
        "toint",
        inp2.var
    );
    assert(inp.error == OK && inp2.error == OK && "I told you should input only integer :(");
    Variable pipe = inp.var.container[0] + inp2.var.container[0];
    ArrayWithCode _ = EExecFunc("out", Array(pipe));
}
