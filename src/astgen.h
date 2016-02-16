// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#ifndef ASTGEN_H
#define ASTGEN_H

#include "syntax.h"

struct astgen: public syntax::delegate {
	virtual void rule_0_empty() override;
	virtual void rule_0_number(std::string) override;
	virtual void rule_0_symbol(std::string) override;
	virtual void rule_0_string(std::string) override;
	virtual void rule_0_placeholder() override;
	virtual void rule_2_sequence() override;
	virtual void rule_2_capture() override;
	virtual void rule_2_define() override;
	virtual void rule_2_join() override;
	virtual void rule_2_caption() override;
	virtual void rule_2_equal() override;
	virtual void rule_2_lesser() override;
	virtual void rule_2_greater() override;
	virtual void rule_2_not_equal() override;
	virtual void rule_2_not_lesser() override;
	virtual void rule_2_not_greater() override;
	virtual void rule_2_add() override;
	virtual void rule_2_subtract() override;
	virtual void rule_2_or() override;
	virtual void rule_2_xor() override;
	virtual void rule_2_range() override;
	virtual void rule_2_multiplication() override;
	virtual void rule_2_division() override;
	virtual void rule_2_modulo() override;
	virtual void rule_2_shift_left() override;
	virtual void rule_2_shift_right() override;
	virtual void rule_2_and() override;
	virtual void rule_1_negate() override;
	virtual void rule_1_complement() override;
	virtual void rule_1_eval() override;
	virtual void rule_1_list() override;
	virtual void rule_1_object() override;
	virtual void rule_2_subscript() override;
	virtual void rule_2_lookup() override;
};

#endif //ASTGEN_H

