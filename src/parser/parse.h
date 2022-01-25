/*
 * src/parser/parse.h
 * Wopslang Parsing Headerfile
 *
 * 2021, Wops Team
 * */

#include <utility>
#include <vector>
#include <string>
#include <regex>
#include <queue>
#include "../runtime/AST.h"
#include "../error/signal.h"

#ifndef WOPS_PARSE_H
#define WOPS_PARSE_H

void Parse(AST& head, std::vector<String> codes);
Expr ParseExpr(std::vector<std::string> tokens, int parsing_line);

#endif
