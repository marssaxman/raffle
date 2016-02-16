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
	virtual location loc() { return location(); }
};

extern node empty;

struct literal: public node {
	typedef enum {
		number, string
	} opcode;
	opcode id;
	std::string text;
	literal(opcode o, std::string t, location l);
	virtual location loc() override;
private:
	location tk_loc;
};

struct symbol: public node {
	std::string text;
	symbol(std::string t, location l);
	virtual location loc() override;
private:
	location tk_loc;
};

struct placeholder: public node {
	placeholder(location l): tk_loc(l) {}
	virtual location loc() override { return tk_loc; }
private:
	location tk_loc;
};

struct invocation: public node {
	typedef enum {
		subscript, lookup, caption
	} opcode;
	opcode id;
	node &target;
	node &argument;
	invocation(opcode o, node &t, node &a): target(t), argument(a) {}
	virtual location loc() override { return target.loc() + argument.loc(); }
};

struct definition: public node {
	typedef enum {
		evaluate, capture
	} opcode;
	opcode id;
	node &sym;
	node &exp;
	definition(opcode o, node &s, node &e): sym(s), exp(e) {}
	virtual location loc() override { return sym.loc() + exp.loc(); }
};

struct binary: public node {
	node &left;
	node &right;
	binary(node &l, node &r): left(l), right(r) {}
	virtual location loc() override { return left.loc() + right.loc(); }
};

struct arithmetic: public binary {
	typedef enum {
		add, subtract, multiply, divide, modulo, shift_left, shift_right,
	} opcode;
	opcode id;
	arithmetic(opcode o, node &l, node &r): binary(l, r), id(o) {}
};

struct logic: public binary {
	typedef enum {
		conjoin, disjoin, exclude,
	} opcode;
	opcode id;
	logic(opcode o, node &l, node &r): binary(l, r), id(o) {}
};

struct relation: public binary {
	typedef enum {
		equal, greater, lesser, not_equal, not_greater, not_lesser,
	} opcode;
	opcode id;
	relation(opcode o, node &l, node &r): binary(l, r), id(o) {}
};

struct range: public binary {
	range(node &l, node &r): binary(l, r) {}
};

struct join: public node {
	node &exp;
	node &next;
	join(node &e, node &n): exp(e), next(n) {}
	virtual location loc() override { return exp.loc() + next.loc(); }
};

struct sequence: public node {
	node &exp;
	node &next;
	sequence(node &e, node &n): exp(e), next(n) {}
	virtual location loc() override { return exp.loc() + next.loc(); }
};

struct unary: public node {
	typedef enum {
		negate, complement,
	} opcode;
	opcode id;
	node &source;
	unary(opcode o, node &s, location l);
	virtual location loc() override;
private:
	location tk_loc;
};

struct constructor: public node {
	typedef enum {
		tuple, list, object
	} opcode;
	opcode id;
	node &items;
	constructor(opcode o, node &i, location l);
	virtual location loc() override;
private:
	location tk_loc;
};

} // namespace ast

#endif //AST_H

