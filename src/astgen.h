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
	virtual void rule_0_number(location, std::string) override;
	virtual void rule_0_symbol(location, std::string) override;
	virtual void rule_0_string(location, std::string) override;
	virtual void rule_0_placeholder(location) override;
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
	virtual void rule_2_multiply() override;
	virtual void rule_2_divide() override;
	virtual void rule_2_modulo() override;
	virtual void rule_2_shift_left() override;
	virtual void rule_2_shift_right() override;
	virtual void rule_2_and() override;
	virtual void rule_1_negate(location) override;
	virtual void rule_1_complement(location) override;
	virtual void rule_1_eval(location) override;
	virtual void rule_1_list(location) override;
	virtual void rule_1_object(location) override;
	virtual void rule_2_subscript() override;
	virtual void rule_2_lookup() override;
};

#endif //ASTGEN_H

