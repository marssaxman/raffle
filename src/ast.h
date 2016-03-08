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

enum class atom {
	wildcard, null
};
enum class leaf {
	number, string, identifier
};
enum class branch {
	apply, pipe, sequence,
	pair, range, conjoin, disjoin, exclude,
	assign, capture, declare, define, typealias,
	add, sub, mul, div, rem, shl, shr, eq, gt, lt, neq, ngt, nlt,
};
struct delegate {
	virtual void ast_atom(location, atom) = 0;
	virtual void ast_leaf(location, leaf, std::string) = 0;
	virtual void ast_branch(location, branch, std::string) = 0;
};

} // namespace ast

#endif //AST_H

