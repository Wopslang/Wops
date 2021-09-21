/*
 * src/parser/parse.h
 * Wopslang Parsing Headerfile
 *
 * 2021, Wops Team
 * */

#include <utility>
#include <vector>
#include <string>
#include "../runtime/AST.h"
#include "../error/signal.h"

#ifndef WOPS_PARSE_H
#define WOPS_PARSE_H

AST Parse(std::vector<std::string> codes);

#endif
