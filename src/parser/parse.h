/*
 * src/parser/parse.h
 * Wopslang Parsing Headerfile
 *
 * 2023, Wops Team
 * */

#include <algorithm>
#include <queue>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "../error/signal.h"
#include "../runtime/AST.h"

#ifndef WOPS_PARSE_H
#define WOPS_PARSE_H

std::pair<int, int> Parse(AST& head, std::vector<std::vector<String>> Token_table, int parse_start, int arg_idx, std::vector<String>& token_storage);
Expr ParseExpr(std::vector<std::string> tokens, int parsing_line);
std::vector<String> GetTokenTable(String code);

#endif