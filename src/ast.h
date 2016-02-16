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

struct subscript: public node {
	typedef enum {
		apply, select, expand
	} opcode;
	opcode id;
	node &target;
	node &argument;
	subscript(opcode o, node &t, node &a, location l);
	virtual location loc() override;
private:
	location tk_loc;
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
	binary(opcode o, node &l, node &r);
	virtual location loc() override;
};

struct unary: public node {
	typedef enum {
		negate, complement,
		list, object,
	} opcode;
	opcode id;
	node &source;
	unary(opcode o, node &s, location l);
	virtual location loc() override;
private:
	location tk_loc;
};

} // namespace ast

#endif //AST_H

