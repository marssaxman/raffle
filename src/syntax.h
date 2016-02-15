// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef SYNTAX_H
#define SYNTAX_H

#include <string>

namespace syntax {
struct delegate {
	virtual int rule_empty() = 0; // for optional items
	virtual int rule_number(std::string) = 0;
	virtual int rule_symbol(std::string) = 0;
	virtual int rule_string(std::string) = 0;
	virtual int rule_placeholder() = 0; // 
	virtual int rule_sequence(int, int) = 0;
	virtual int rule_capture(int, int) = 0;
	virtual int rule_define(int, int) = 0;
	virtual int rule_join(int, int) = 0;
	virtual int rule_caption(int, int) = 0;
	virtual int rule_equal(int, int) = 0;
	virtual int rule_lesser(int, int) = 0;
	virtual int rule_greater(int, int) = 0;
	virtual int rule_not_equal(int, int) = 0;
	virtual int rule_not_lesser(int, int) = 0;
	virtual int rule_not_greater(int, int) = 0;
	virtual int rule_addition(int, int) = 0;
	virtual int rule_subtraction(int, int) = 0;
	virtual int rule_or(int, int) = 0;
	virtual int rule_xor(int, int) = 0;
	virtual int rule_range(int, int) = 0;
	virtual int rule_multiplication(int, int) = 0;
	virtual int rule_division(int, int) = 0;
	virtual int rule_modulo(int, int) = 0;
	virtual int rule_shift_left(int, int) = 0;
	virtual int rule_shift_right(int, int) = 0;
	virtual int rule_and(int, int) = 0;
	virtual int rule_negate(int) = 0;
	virtual int rule_complement(int) = 0;
	virtual int rule_eval(int) = 0;
	virtual int rule_list(int) = 0;
	virtual int rule_object(int) = 0;
	virtual int rule_subscript(int, int) = 0;
	virtual int rule_lookup(int, int) = 0;
};
} // namespace syntax

#endif //SYNTAX_H

