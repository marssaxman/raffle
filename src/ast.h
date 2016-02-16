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

struct visitor;

struct node {
	virtual ~node() {}
	virtual void accept(visitor&) = 0;
	virtual location loc() = 0;
};

struct empty: public node {
	virtual void accept(visitor&) override;
	virtual location loc() override { return location{}; }
};

struct literal: public node {
	typedef enum {
		number, string
	} opcode;
	opcode id;
	std::string text;
	literal(opcode o, std::string t, location l);
	virtual void accept(visitor &v) override;
	virtual location loc() override;
private:
	location tk_loc;
};

struct symbol: public node {
	std::string text;
	symbol(std::string t, location l);
	virtual void accept(visitor &v) override;
	virtual location loc() override;
private:
	location tk_loc;
};

struct placeholder: public node {
	placeholder(location l): tk_loc(l) {}
	virtual void accept(visitor &v) override;
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
	virtual void accept(visitor &v) override;
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
	virtual void accept(visitor &v) override;
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
	virtual void accept(visitor &v) override;
};

struct logic: public binary {
	typedef enum {
		conjoin, disjoin, exclude,
	} opcode;
	opcode id;
	logic(opcode o, node &l, node &r): binary(l, r), id(o) {}
	virtual void accept(visitor &v) override;
};

struct relation: public binary {
	typedef enum {
		equal, greater, lesser, not_equal, not_greater, not_lesser,
	} opcode;
	opcode id;
	relation(opcode o, node &l, node &r): binary(l, r), id(o) {}
	virtual void accept(visitor &v) override;
};

struct range: public binary {
	range(node &l, node &r): binary(l, r) {}
	virtual void accept(visitor &v) override;
};

struct join: public node {
	node &exp;
	node &next;
	join(node &e, node &n): exp(e), next(n) {}
	virtual location loc() override { return exp.loc() + next.loc(); }
	virtual void accept(visitor &v) override;
};

struct sequence: public node {
	node &exp;
	node &next;
	sequence(node &e, node &n): exp(e), next(n) {}
	virtual location loc() override { return exp.loc() + next.loc(); }
	virtual void accept(visitor &v) override;
};

struct invert: public node {
	typedef enum {
		negate, complement,
	} opcode;
	opcode id;
	node &source;
	invert(opcode o, node &s, location l): id(o), source(s), tk_loc(l) {}
	virtual location loc() override { return tk_loc + source.loc(); }
	virtual void accept(visitor &v) override;
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
	virtual void accept(visitor &v) override;
private:
	location tk_loc;
};

struct visitor {
	virtual void visit(empty&) = 0;
	virtual void visit(literal&) = 0;
	virtual void visit(symbol&) = 0;
	virtual void visit(placeholder&) = 0;
	virtual void visit(invocation&) = 0;
	virtual void visit(definition&) = 0;
	virtual void visit(arithmetic&) = 0;
	virtual void visit(logic&) = 0;
	virtual void visit(relation&) = 0;
	virtual void visit(range&) = 0;
	virtual void visit(join&) = 0;
	virtual void visit(sequence&) = 0;
	virtual void visit(invert&) = 0;
	virtual void visit(constructor&) = 0;
};

} // namespace ast

#endif //AST_H

