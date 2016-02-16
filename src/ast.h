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

struct node {
	virtual location loc() = 0;
};

struct binary: public node {
	typedef enum {
		add, subtract, multiply, divide, modulo, shift_left, shift_right,
		conjoin, disjoin, exclude,
		equal, greater, lesser, not_equal, not_greater, not_lesser,
	} opcode;
	opcode id;
	node &left;
	node &right;
	binary(opcode o, node &l, node &r): id(o), left(l), right(r) {}
	virtual location loc() override { return left.loc() + right.loc(); }
};

struct unary: public node {
	typedef enum {
		negate, complement
	} opcode;
	opcode id;
	node &source;
	unary(opcode o, node &s, location l): id(o), source(s), tk_loc(l) {}
	virtual location loc() override { return tk_loc + source.loc(); }
private:
	location tk_loc;
};

} // namespace ast

#endif //AST_H

