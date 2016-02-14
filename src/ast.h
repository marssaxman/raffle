// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef AST_H
#define AST_H

#include "parser.h"
#include <memory>

namespace ast {

struct node {
	enum type {
		empty,
		number,
		symbol,
		string,
		placeholder,
	} id;
	node(type i): id(i) {}
};

typedef const node &ref;

struct leaf: public node {
	std::string data;
	leaf(type i, std::string d): node(i), data(d) {}
};

struct twig: public node {
	ref source;
	twig(type i, ref s): node(i), source(s) {}
};

struct branch: public node {
	ref left;
	ref right;
	branch(type i, ref l, ref r): node(i), left(l), right(r) {}
};

typedef std::vector<std::unique_ptr<node>> tree;

struct builder: public parser::output {
	builder() { make(node::empty); }
	virtual int rule_empty() override;
	virtual int rule_number(std::string t) override;
	virtual int rule_symbol(std::string t) override;
	virtual int rule_string(std::string t) override;
	virtual int rule_placeholder() override;
	virtual int rule_sequence(int l, int r) override;
	virtual int rule_capture(int, int) override;
	virtual int rule_define(int, int) override;
	virtual int rule_join(int l, int r) override;
	virtual int rule_caption(int l, int r) override;
	virtual int rule_equal(int l, int r) override;
	virtual int rule_lesser(int l, int r) override;
	virtual int rule_greater(int l, int r) override;
	virtual int rule_addition(int l, int r) override;
	virtual int rule_subtraction(int l, int r) override;
	virtual int rule_or(int l, int r) override;
	virtual int rule_xor(int l, int r) override;
	virtual int rule_range(int l, int r) override;
	virtual int rule_multiplication(int l, int r) override;
	virtual int rule_division(int l, int r) override;
	virtual int rule_modulo(int l, int r) override;
	virtual int rule_shift_left(int l, int r) override;
	virtual int rule_shift_right(int l, int r) override;
	virtual int rule_and(int l, int r) override;
	virtual int rule_negate(int v) override;
	virtual int rule_complement(int v) override;
	virtual int rule_eval(int v) override;
	virtual int rule_list(int v) override;
	virtual int rule_object(int v) override;
	virtual int rule_subscript(int, int) override;
	virtual int rule_lookup(int, int) override;
private:
	tree t;
	int make(node::type);
	int make(node::type, std::string);
	int make(node::type, ref);
	int make(node::type, ref, ref);
	unsigned i = 0;
};

} // namespace AST

#endif //AST_H

