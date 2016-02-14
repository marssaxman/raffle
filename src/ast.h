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
};

typedef const node &ref;

struct leaf: public node {
	std::string data;
};

struct twig: public node {
	ref source;
};

struct branch: public node {
	ref left;
	ref right;
};

struct builder: public parser::output {
	virtual int rule_number(std::string t) override;
	virtual int rule_symbol(std::string t) override;
	virtual int rule_string(std::string t) override;
	virtual int rule_blank() override;
	virtual int rule_paren_empty() override;
	virtual int rule_bracket_empty() override;
	virtual int rule_brace_empty() override;
	virtual int rule_sequence(int l, int r) override;
	virtual int rule_capture(int, int) override;
	virtual int rule_define(int, int) override;
	virtual int rule_list(int l, int r) override;
	virtual int rule_caption(int l, int r) override;
	virtual int rule_equal(int l, int r) override;
	virtual int rule_lesser(int l, int r) override;
	virtual int rule_greater(int l, int r) override;
	virtual int rule_not_equal(int l, int r) override;
	virtual int rule_not_lesser(int l, int r) override;
	virtual int rule_not_greater(int l, int r) override;
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
	virtual int rule_paren_group(int v) override;
	virtual int rule_bracket_group(int v) override;
	virtual int rule_brace_group(int v) override;
	virtual int rule_subscript(int, int) override;
	virtual int rule_lookup(int, int) override;
private:
	unsigned i = 0;
};

} // namespace AST

#endif //AST_H

