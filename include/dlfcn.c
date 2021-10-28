/*
 * include/dlfcn.cpp
 * Wopslang DLFCN-WIN Sourcefile
 *
 * 2021, Wops Team
 * */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct CACHE_DLFCN{
    long lasterror;
    const char *err_rutin;
} CACHE_DLFCN;

CACHE_DLFCN cache = {
    0,
    NULL
};

void *dlopen(const char *filename, int flags) {
    HINSTANCE h_instance;

    h_instance = LoadLibrary(filename);
    if (h_instance == NULL) {
        cache.lasterror = GetLastError();
        cache.err_rutin = "dlopen";
    }
    return h_instance;
}

int dlclose(void *handle) {
    BOOL status;
    int return_code = 0;

    status = FreeLibrary(
        (HINSTANCE)handle
    );

    if (!status) {
        cache.lasterror = GetLastError();
        cache.err_rutin = "dlclose";
        return_code = -1;
    }

    return return_code;
}

void *dlsym(void *handle, const char *name) {
    FARPROC func_ptr;

    func_ptr = GetProcAddress(
        (HINSTANCE)handle,
        name
    );

    if (!func_ptr) {
        func_ptr.lasterror = GetLastError();
        func_ptr.err_rutin = "dlsym";
    }
    return (void *)(intptr_t) func_ptr;
}

const char *dlerror(void) {
    static char* errstr;
    if (cache.lasterror) {
        sprintf(errstr, "%s error (%ld)", cache.err_rutin, cache.lasterror);
        return errstr;
    } else {
        return NULL;
    }
}

