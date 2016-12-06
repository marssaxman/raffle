// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef SYNTAX_H
#define SYNTAX_H

#include "location.h"
#include <string>

namespace syntax {
enum branch {
	apply, pipe, sequence, pair, range,
	assign, capture, declare, define, typealias,
	and_join, or_join, xor_join, nand_join, nor_join, xnor_join,
	add, sub, mul, div, rem, shl, shr, eq, gt, lt, neq, ngt, nlt
};
struct delegate {
	virtual void emit_eof(location) = 0;
	virtual void emit_wildcard(location) = 0;
	virtual void emit_null(location) = 0;
	virtual void emit_number(std::string, location) = 0;
	virtual void emit_string(std::string, location) = 0;
	virtual void emit_identifier(std::string, location) = 0;
	virtual void emit_branch(enum branch, std::string, location) = 0;
};
} // namespace syntax

#endif //SYNTAX_H

