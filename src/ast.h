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
#include <list>

namespace ast {

struct visitor;
struct node;
typedef std::unique_ptr<node> ptr;
struct delegate {
	virtual void ast_process(ptr &&) = 0;
	virtual void ast_done() = 0;
};

struct node {
	virtual ~node() {}
	virtual void accept(visitor&) = 0;
	virtual location loc() = 0;
};

struct number: public node {
	std::string text;
	number(std::string t, location l): text(t), tk_loc(l) {}
	virtual void accept(visitor &v) override;
	virtual location loc() override { return tk_loc; }
private:
	location tk_loc;
};

struct string: public node {
	std::string text;
	string(std::string t, location l): text(t), tk_loc(l) {}
	virtual void accept(visitor &v) override;
	virtual location loc() override { return tk_loc; }
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

struct wildcard: public node {
	wildcard(location l): tk_loc(l) {}
	virtual void accept(visitor &v) override;
	virtual location loc() override { return tk_loc; }
private:
	location tk_loc;
};

struct binary: public node {
	ptr left;
	ptr right;
	binary(ptr &&l, ptr &&r);
	virtual location loc() override { return left->loc() + right->loc(); }
};

struct apply: public binary {
	apply(ptr &&t, ptr &&a);
	virtual void accept(visitor &v) override;
};

struct pipeline: public binary {
	pipeline(ptr &&a, ptr &&t);
	virtual void accept(visitor &v) override;
};

struct assign: public binary {
	assign(ptr &&t, ptr &&a);
	virtual void accept(visitor &v) override;
};

struct capture: public binary {
	capture(ptr &&t, ptr &&a);
	virtual void accept(visitor &v) override;
};

struct define: public binary {
	define(ptr &&t, ptr &&a);
	virtual void accept(visitor &v) override;
};

struct operate: public binary {
	typedef enum {
		add, sub, mul, div, rem, shl, shr,
		conjoin, disjoin, exclude,
		eq, gt, lt, neq, ngt, nlt,
	} opcode;
	opcode id;
	operate(opcode o, ptr &&l, ptr &&r);
	virtual void accept(visitor &v) override;
};

struct range: public binary {
	range(ptr &&l, ptr &&r);
	virtual void accept(visitor &v) override;
};

struct negate: public node {
	typedef enum {
		numeric, logical,
	} opcode;
	opcode id;
	ptr source;
	negate(opcode o, ptr &&s, location l);
	virtual location loc() override { return tk_loc + source->loc(); }
	virtual void accept(visitor &v) override;
private:
	location tk_loc;
};

struct tuple: public binary {
	tuple(ptr &&l, ptr &&r);
	virtual void accept(visitor &v) override;
};

struct group: public node {
	typedef enum {
		value, construct, scope
	} opcode;
	opcode id;
	std::list<ptr> items;
	group(opcode o, std::list<ptr> &&i, location l);
	virtual location loc() override { return tk_loc; }
	virtual void accept(visitor &v) override;
private:
	location tk_loc;
};

struct visitor {
	virtual void visit(number&) = 0;
	virtual void visit(string&) = 0;
	virtual void visit(symbol&) = 0;
	virtual void visit(wildcard&) = 0;
	virtual void visit(apply&) = 0;
	virtual void visit(compose&) = 0;
	virtual void visit(assign&) = 0;
	virtual void visit(capture&) = 0;
	virtual void visit(define&) = 0;
	virtual void visit(operate&) = 0;
	virtual void visit(range&) = 0;
	virtual void visit(negate&) = 0;
	virtual void visit(tuple&) = 0;
	virtual void visit(group&) = 0;
};

} // namespace ast

#endif //AST_H

