/*
 * include/dlfcn.h
 * Wopslang DLFCN-WIN headerfile 
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_DLFCN_H
#define WOPS_DLFCN_H

// Redefine the plag
#define RTLD_GLOBAL 0x100
#define RTLD_LOCAL  0x000
#define RTLD_LAZY   0x000
#define RTLD_NOW    0x001

#ifdef __cplusplus
extern "C" {
#endif

    void *dlopen  (const char *filename, int flag);
    int   dlclose (void *handle);
    void *dlsym   (void *handle, const char *name);
    const char *dlerror (void);

#ifdef __cplusplus
}
#endif

#endif
