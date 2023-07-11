/*
 * src/import_/eexec_.cpp
 * Wopslang External Function Executor Sourcefile
 *
 * 2021, Wops Team
 * */


#include "eexec_.h"

// Object EExecFunc(std::string func, Object argv)
// Calls a function with argv as a parameter and func as a name.
Object EExecFunc(std::string func, Object argv) {
    // EMPTY VARIABLE
    Object null("_", {}, {}, Variable("_", "", INT), 0);

    void* handle = dlopen("lib/library.so", RTLD_LAZY);

    if (!handle) {
        ErrHandler().CallErr(-1, CANNOT_LOAD_LIBRARY, {std::string(dlerror())});
        null.errvalue = CANNOT_LOAD_LIBRARY;
        return null;
    }

    // function pointer
    typedef Object (*t)(Object);

    // reset errors
    dlerror();

    std::string exfunc(func);
    t fptr = (t) dlsym(handle, exfunc.c_str());
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        ErrHandler().CallErr(-1, CANNOT_LOAD_SYMBOL, {func});
        dlclose(handle);
        null.errvalue = CANNOT_LOAD_SYMBOL;
        return null;
    }

    Object ret = fptr(argv);
    dlclose(handle);
    return ret;
}
