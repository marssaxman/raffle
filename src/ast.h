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

struct group: public node {
	ptr source;
	group(location l, ptr &&s, location r);
	virtual location loc() override { return openloc + closeloc; }
private:
	location openloc;
	location closeloc;
};

struct parens: public group {
	using group::group;
	virtual void accept(visitor &v) override;
};

struct brackets: public group {
	using group::group;
	virtual void accept(visitor &v) override;
};

struct braces: public group {
	using group::group;
	virtual void accept(visitor &v) override;
};

struct binary: public node {
	typedef enum {
		sequence, tuple, range,
		apply, pipeline, assign, capture, declare, define, typealias,
		add, sub, mul, div, rem, shl, shr,
		conjoin, disjoin, exclude,
		eq, gt, lt, neq, ngt, nlt,
	} opcode;
	opcode id;
	ptr left;
	ptr right;
	binary(opcode o, ptr &&l, ptr &&r);
	virtual location loc() override { return left->loc() + right->loc(); }
	virtual void accept(visitor &v) override;
};

struct visitor {
	virtual void visit(node&) {}
	virtual void visit(number &n) { visit((node&)n); }
	virtual void visit(string &n) { visit((node&)n); }
	virtual void visit(symbol &n) { visit((node&)n); }
	virtual void visit(wildcard &n) { visit((node&)n); }
	virtual void visit(group &n) { visit((node&)n); }
	virtual void visit(parens &n) { visit((group&)n); }
	virtual void visit(brackets &n) { visit((group&)n); }
	virtual void visit(braces &n) { visit((group&)n); }
	virtual void visit(binary &n) { visit((node&)n); }
};

struct ostream {
	virtual ostream &operator<<(ptr&&) = 0;
};

struct processor: public ostream, protected visitor {
	processor(ostream &o): out(o) {}
	virtual ostream &operator<<(ptr&&) override;
protected:
	void send(ptr &&n);
private:
	ptr replace;
	ostream &out;
};

} // namespace ast

#endif //AST_H

