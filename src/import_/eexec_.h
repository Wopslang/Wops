/*
 * src/import_/eexec_.h
 * Wopslang External Function Executor Headerfile
 *
 * 2023, Wops Team
 * */

#ifdef _WIN32
#include "../../include/dlfcn.h"
#else
#include <dlfcn.h>
#endif

#include <iostream>
#include <string>
#include "../type/variable.h"
#include "../type/object.h"
#include "../error/signal.h"

#ifndef WOPS_EEXEC_H
#define WOPS_EEXEC_H

/*
Object EExecFunc(std::string func, Object argv)
Calls a function with argv as a parameter and func as a name.
*/
Object EExecFunc(std::string func, Object argv);

#endif
