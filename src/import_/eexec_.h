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
#include "../type/variable.h"

/*
VariableWithCode EExecFunc(std::string func, Variable argv)
함수 이름이 func인 함수에 argv를 매개변수로 두어 호출하는 함수
*/
VariableWithCode EExecFunc(std::string func, Variable argv);

#endif