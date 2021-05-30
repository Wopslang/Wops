/*
 * src/import_/eexec_.h
 * Wopslang External Function Executor Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_EEXEC_H
#define WOPS_EEXEC_H

#include <dlfcn.h>
#include <iostream>
#include <string>
#include "src/type/variable.h"

/*
VariableWithCode EExecFunc(std::string func, Variable argv);
Execute External Function
*/
VariableWithCode EExecFunc(std::string func, Variable argv);

#endif