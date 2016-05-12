// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef AST_H
#define AST_H

#include "location.h"
#include <string>

namespace ast {
enum type {
	// atoms: zero edges
	eof = (0<<8), wildcard, null,
	// leafs: one edge
	number = (1<<8), string, identifier,
	// branches: two edges
	apply = (2<<8), pipe, sequence, pair, range,
	assign, capture, declare, define, typealias,
	and_join, or_join, xor_join, nand_join, nor_join, xnor_join,
	add, sub, mul, div, rem, shl, shr, eq, gt, lt, neq, ngt, nlt
};
static inline int edges(ast::type id) {
	return static_cast<int>(id) >> 8;
}
struct builder {
	virtual void emit(enum type, std::string, location) = 0;
};
} // namespace ast

#endif //AST_H

