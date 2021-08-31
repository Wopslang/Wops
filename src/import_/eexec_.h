/*
 * src/import_/eexec_.h
 * Wopslang External Function Executor Headerfile
 *
 * 2021, Wops Team
 * */

#include <dlfcn.h>
#include <iostream>
#include "../type/variable.h"
#include "../type/array.h"
#include "../error/signal.h"

#ifndef WOPS_EEXEC_H
#define WOPS_EEXEC_H

/*
ArrayWithCode EExecFunc(std::string func, Array argv)
Calls a function with argv as a parameter and func as a name.
*/
ArrayWithCode EExecFunc(std::string func, Array argv);

#endif
