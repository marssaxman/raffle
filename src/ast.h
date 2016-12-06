// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef AST_H
#define AST_H

#include "location.h"
#include <memory>
#include <string>

namespace ast {

struct visitor;
struct node;
typedef std::unique_ptr<node> unique_ptr;

struct node {
	node(location o): origin(o) {}
	virtual void accept(visitor&) const = 0;
	location origin;
};

struct eof: public node {
	using node::node;
	virtual void accept(visitor&) const override;
};

struct wildcard: public node {
	using node::node;
	virtual void accept(visitor&) const override;
};

struct null: public node {
	using node::node;
	virtual void accept(visitor&) const override;
};

struct leaf: public node {
	leaf(std::string t, location o): node(o), text(t) {}
	std::string text;
};

struct number: public leaf {
	using leaf::leaf;
	virtual void accept(visitor&) const override;
};

struct string: public leaf {
	using leaf::leaf;
	virtual void accept(visitor&) const override;
};

struct identifier: public leaf {
	using leaf::leaf;
	virtual void accept(visitor&) const override;
};

struct branch: public node {
	branch(unique_ptr &&l, unique_ptr &&r, location o):
			node(o), left(std::move(l)), right(std::move(r)) {}
	unique_ptr left;
	unique_ptr right;
};

struct apply: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct pipe: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct sequence: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct pair: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct range: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct assign: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct capture: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct declare: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct define: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct typealias: public branch {
	using branch::branch;
	virtual void accept(visitor&) const override;
};

struct binop: public branch {
	binop(std::string t, unique_ptr &&l, unique_ptr &&r, location o):
			branch(std::move(l), std::move(r), o), text(t) {}
	virtual void accept(visitor&) const override;
	std::string text;
};

struct visitor {
	virtual void visit(const eof&) = 0;
	virtual void visit(const wildcard&) = 0;
	virtual void visit(const null&) = 0;
	virtual void visit(const number&) = 0;
	virtual void visit(const string&) = 0;
	virtual void visit(const identifier&) = 0;
	virtual void visit(const apply&) = 0;
	virtual void visit(const pipe&) = 0;
	virtual void visit(const sequence&) = 0;
	virtual void visit(const pair&) = 0;
	virtual void visit(const range&) = 0;
	virtual void visit(const assign&) = 0;
	virtual void visit(const capture&) = 0;
	virtual void visit(const declare&) = 0;
	virtual void visit(const define&) = 0;
	virtual void visit(const typealias&) = 0;
	virtual void visit(const binop&) = 0;
};

struct delegate {
	virtual void process(unique_ptr&&) = 0;
};

} // namespace ast

#endif //AST_H

