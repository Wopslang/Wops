/*
 * src/type/operator.h
 * Wopslang Variable Operation Master Headerfile
 *
 * 2023, Wops Team
 * */

#ifndef WOPS_OPERATOR_H
#define WOPS_OPERATOR_H
#include <utility>
#include "variable.h"

enum op_ {
	// Arithmetic
	PLU, // +
	MIN, // -
	MUL, // *
	DIV, // /
	MOD, // %
	ASSIGN, // =

	// Relational
	EQ, // ==
	NEQ, // !=
	LG, // >
	SM, // <
	LGE, // >=
	SME, // <=

	// Logical
	LNOT, // !
	LAND, // &&
	LOR, // ||
};
#endif
