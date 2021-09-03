/*
 * src/import_/eexec_.cpp
 * Wopslang External Function Executor Sourcefile
 *
 * 2021, Wops Team
 * */


#include "eexec_.h"

// ArrayWithCode EExecFunc(std::string func, Array argv)
// Calls a function with argv as a parameter and func as a name.
ArrayWithCode EExecFunc(std::string func, Array argv) {
    // EMPTY VARIABLE
    Array null(Variable("_", "", INT));

    void* handle = dlopen("src/import_/dll/library.so", RTLD_LAZY);

    if (!handle) {
        ErrHandler().CallErr("Cannot load library");
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
        ErrHandler().CallErr("Cannot load symbol: " + func);
        dlclose(handle);
        return {null, ERROR};
    }

    ArrayWithCode ret = fptr(argv);
    dlclose(handle);
    return ret;
}