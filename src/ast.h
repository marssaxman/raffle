// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef AST_H
#define AST_H

#include "location.h"
#include <string>
#include <memory>

// An abstract syntax tree is a representation of a program's syntactic
// structure which omits details not relevant for semantic analysis.
namespace ast {

struct node;
typedef std::unique_ptr<node> ptr;
struct visitor;

struct node {
	virtual ~node() {}
	virtual void accept(visitor&) = 0;
	virtual location loc() = 0;
};

struct leaf: public node {
	typedef enum {
		number, string, symbol
	} tag;
	tag id;
	std::string text;
	leaf(tag o, std::string t, location l): id(o), text(t), tk_loc(l) {}
	virtual void accept(visitor &v) override;
	virtual location loc() override { return tk_loc; }
private:
	location tk_loc;
};

struct branch: public node {
	typedef enum {
		sequence, tuple, range,
		apply, pipeline, assign, capture, declare, define, typealias,
		add, sub, mul, div, rem, shl, shr,
		conjoin, disjoin, exclude,
		eq, gt, lt, neq, ngt, nlt,
	} tag;
	tag id;
	ptr left;
	ptr right;
	branch(tag o, ptr &&l, ptr &&r);
	virtual location loc() override { return left->loc() + right->loc(); }
	virtual void accept(visitor &v) override;
};

struct visitor {
	virtual void visit(leaf&) {}
	virtual void visit(branch&) {}
};

struct builder {
	virtual void build_blank(location) = 0;
	virtual void build_leaf(ast::leaf::tag, std::string, location) = 0;
	virtual void build_branch(ast::branch::tag, location) = 0;
};

} // namespace ast

#endif //AST_H

