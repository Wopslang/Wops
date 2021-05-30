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

/*
void EExecFunc(std::string func, ARG_T argv)
call external function
*/
template <typename ARG_T, typename RETURN_T>
RETURN_T EExecFunc(std::string func, ARG_T argv);

#endif