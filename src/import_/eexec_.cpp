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
// 함수 이름이 func인 함수에 argv를 매개변수로 두어 호출하는 함수
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

// unit test: out(in())
int main() {
    ArrayWithCode inp = EExecFunc("in", Array(Variable("_", "", STRING)));
    ArrayWithCode _ = EExecFunc("out", inp.var);
}