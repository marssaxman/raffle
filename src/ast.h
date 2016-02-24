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
	location nodeloc() { return tk_loc; }
	virtual location treeloc() { return nodeloc(); }
protected:
	node(location l): tk_loc(l) {}
private:
	location tk_loc;
};

struct atom: public node {
	typedef enum {
		wildcard, null
	} tag;
	tag id;
	atom(location loc, tag i): node(loc), id(i) {}
	using node::node;
	virtual void accept(visitor &v) override;
};

struct leaf: public node {
	typedef enum {
		number, string, symbol
	} tag;
	tag id;
	std::string text;
	leaf(location loc, tag i, std::string t): node(loc), id(i), text(t) {}
	virtual void accept(visitor &v) override;
};

struct branch: public node {
	typedef enum {
		sequence, pair, range, conjoin, disjoin, exclude,
		apply, invoke, slice, extend, pipeline,
		assign, capture, declare, define, typealias,
		add, sub, mul, div, rem, shl, shr, eq, gt, lt, neq, ngt, nlt,
	} tag;
	tag id;
	ptr left;
	ptr right;
	branch(location, tag, ptr &&l, ptr &&r);
	virtual location treeloc() override;
	virtual void accept(visitor &v) override;
};

struct visitor {
	virtual void visit(atom&) {}
	virtual void visit(leaf&) {}
	virtual void visit(branch&) {}
};

struct builder {
	virtual void build_atom(location, atom::tag) = 0;
	virtual void build_leaf(location, leaf::tag, std::string) = 0;
	virtual void build_branch(location, branch::tag) = 0;
};

} // namespace ast

#endif //AST_H

