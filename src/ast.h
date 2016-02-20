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

struct node;
typedef std::unique_ptr<node> ptr;

struct visitor;
struct traversal;

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
	using binary::binary;
	virtual void accept(visitor &v) override;
};

struct pipeline: public binary {
	using binary::binary;
	virtual void accept(visitor &v) override;
};

struct assign: public binary {
	using binary::binary;
	virtual void accept(visitor &v) override;
};

struct capture: public binary {
	using binary::binary;
	virtual void accept(visitor &v) override;
};

struct declare: public binary {
	using binary::binary;
	virtual void accept(visitor &v) override;
};

struct define: public binary {
	using binary::binary;
	virtual void accept(visitor &v) override;
};

struct typealias: public binary {
	using binary::binary;
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
	using binary::binary;
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

struct group: public node {
	typedef enum {
		root, value, spec, scope
	} opcode;
	opcode id;
	location open_loc;
	std::list<ptr> items;
	location close_loc;
	group(opcode o, std::list<ptr> &&i, location l, location r);
	group(opcode o, location l): id(o), open_loc(l), close_loc(l) {}
	virtual location loc() override { return open_loc + close_loc; }
	virtual void accept(visitor &v) override;
};

struct visitor {
	virtual void visit(node&) {}
	virtual void visit(binary &n) { visit((node&)n); }
	virtual void visit(number &n) { visit((node&)n); }
	virtual void visit(string &n) { visit((node&)n); }
	virtual void visit(symbol &n) { visit((node&)n); }
	virtual void visit(wildcard &n) { visit((node&)n); }
	virtual void visit(apply &n) { visit((binary&)n); }
	virtual void visit(pipeline &n) { visit((binary&)n); }
	virtual void visit(assign &n) { visit((binary&)n); }
	virtual void visit(capture &n) { visit((binary&)n); }
	virtual void visit(declare &n) { visit((binary&)n); }
	virtual void visit(define &n) { visit((binary&)n); }
	virtual void visit(typealias &n) { visit((binary&)n); }
	virtual void visit(operate &n) { visit((binary&)n); }
	virtual void visit(range &n) { visit((binary&)n); }
	virtual void visit(negate &n) { visit((node&)n); }
	virtual void visit(group &n) { visit((node&)n); }
};

struct traversal {
	virtual void ast_open(group&) = 0;
	virtual void ast_process(node&) = 0;
	virtual void ast_close(group&) = 0;
};

} // namespace ast

#endif //AST_H

