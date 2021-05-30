/*
 * src/import_/eexec_.cpp
 * Wopslang External Function Executor Sourcefile
 *
 * 2021, Wops Team
 * */

#include <dlfcn.h>
#include <iostream>

template <typename ARG_T, typename RETURN_T>
RETURN_T EExecFunc(std::string func, ARG_T argv) {
    void* handle = dlopen("./library.so", RTLD_LAZY);

    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    typedef RETURN_T (*t)(ARG_T);

    // reset errors
    dlerror();

    std::string exfunc(func);
    t fptr = (t) dlsym(handle, exfunc.c_str());
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol '" << func << "': " << dlsym_error <<
            '\n';
        dlclose(handle);
        return 1;
    }

    int ret = fptr(argv);
    dlclose(handle);
    return ret;
}

// unit test
int main() {
    std::string funcname = "print";
    std::string s = "hello_world";
    int result_code = EExecFunc<std::string, int>(funcname, s);
}