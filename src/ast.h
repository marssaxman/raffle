// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef AST_H
#define AST_H

#include "parser.h"

struct ast: public parser::output {
	virtual int parse_number(std::string t) override;
	virtual int parse_symbol(std::string t) override;
	virtual int parse_string(std::string t) override;
	virtual int parse_paren_empty() override;
	virtual int parse_bracket_empty() override;
	virtual int parse_brace_empty() override;
	virtual int parse_sequence(int l, int r) override;
	virtual int parse_capture(int, int) override;
	virtual int parse_define(int, int) override;
	virtual int parse_list(int l, int r) override;
	virtual int parse_caption(int l, int r) override;
	virtual int parse_equal(int l, int r) override;
	virtual int parse_lesser(int l, int r) override;
	virtual int parse_greater(int l, int r) override;
	virtual int parse_addition(int l, int r) override;
	virtual int parse_subtraction(int l, int r) override;
	virtual int parse_or(int l, int r) override;
	virtual int parse_xor(int l, int r) override;
	virtual int parse_range(int l, int r) override;
	virtual int parse_multiplication(int l, int r) override;
	virtual int parse_division(int l, int r) override;
	virtual int parse_modulo(int l, int r) override;
	virtual int parse_shift_left(int l, int r) override;
	virtual int parse_shift_right(int l, int r) override;
	virtual int parse_and(int l, int r) override;
	virtual int parse_negate(int v) override;
	virtual int parse_paren_group(int v) override;
	virtual int parse_bracket_group(int v) override;
	virtual int parse_brace_group(int v) override;
	virtual int parse_subscript(int, int) override;
private:
	unsigned i = 0;
};

#endif //AST_H

