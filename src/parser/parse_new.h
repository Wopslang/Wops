/*
 * src/parser/parse.h
 * Wopslang Parsing Headerfile
 *
 * 2023, Wops Team
 * */

#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include <regex>
#include <queue>
#include "../runtime/AST.h"
#include "../error/signal.h"

#ifndef WOPS_PARSE_H
#define WOPS_PARSE_H

int Parse(AST& head, std::vector<String> codes);
Expr ParseExpr(std::vector<std::string> tokens, int parsing_line);
std::vector<String> GetTokenTable(String code);

#endif
