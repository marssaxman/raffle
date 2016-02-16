// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef SYNTAX_H
#define SYNTAX_H

#include <string>
#include "location.h"

// Delegate for stack-driven analysis
// Each rule pops either 0, 1, or 2 values from the stack, then pushes one
// value onto the stack (which is implicit, to be managed by the delegate).
// The number of digits to pop is embedded in the method name.
namespace syntax {
struct delegate {
	virtual void rule_0_empty() = 0;
	virtual void rule_0_number(location, std::string) = 0;
	virtual void rule_0_symbol(location, std::string) = 0;
	virtual void rule_0_string(location, std::string) = 0;
	virtual void rule_0_placeholder(location) = 0;
	virtual void rule_2_sequence() = 0;
	virtual void rule_2_capture() = 0;
	virtual void rule_2_define() = 0;
	virtual void rule_2_join() = 0;
	virtual void rule_2_caption() = 0;
	virtual void rule_2_equal() = 0;
	virtual void rule_2_lesser() = 0;
	virtual void rule_2_greater() = 0;
	virtual void rule_2_not_equal() = 0;
	virtual void rule_2_not_lesser() = 0;
	virtual void rule_2_not_greater() = 0;
	virtual void rule_2_add() = 0;
	virtual void rule_2_subtract() = 0;
	virtual void rule_2_or() = 0;
	virtual void rule_2_xor() = 0;
	virtual void rule_2_range() = 0;
	virtual void rule_2_multiply() = 0;
	virtual void rule_2_divide() = 0;
	virtual void rule_2_modulo() = 0;
	virtual void rule_2_shift_left() = 0;
	virtual void rule_2_shift_right() = 0;
	virtual void rule_2_and() = 0;
	virtual void rule_1_negate(location) = 0;
	virtual void rule_1_complement(location) = 0;
	virtual void rule_1_eval(location) = 0;
	virtual void rule_1_list(location) = 0;
	virtual void rule_1_object(location) = 0;
	virtual void rule_2_subscript() = 0;
	virtual void rule_2_lookup() = 0;
};
} // namespace syntax

#endif //SYNTAX_H

