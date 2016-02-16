// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "astgen.h"
#include <iostream>

void astgen::rule_0_empty()
{
}

void astgen::rule_0_number(std::string t)
{
	std::cout << t << " ";
}

void astgen::rule_0_symbol(std::string t)
{
	std::cout << t << " ";
}

void astgen::rule_0_string(std::string t)
{
	std::cout << t << " ";
}

void astgen::rule_0_placeholder()
{
	std::cout << "_ ";
}

void astgen::rule_2_sequence()
{
	std::cout << "; ";
}

void astgen::rule_2_capture()
{
	std::cout << "-> ";
}

void astgen::rule_2_define()
{
	std::cout << "<- ";
}

void astgen::rule_2_join()
{
	std::cout << ", ";
}

void astgen::rule_2_caption()
{
	std::cout << ": ";
}

void astgen::rule_2_equal()
{
	std::cout << "= ";
}

void astgen::rule_2_lesser()
{
	std::cout << "< ";
}

void astgen::rule_2_greater()
{
	std::cout << "> ";
}

void astgen::rule_2_not_equal()
{
	std::cout << "!= ";
}

void astgen::rule_2_not_lesser()
{
	std::cout << "!< ";
}

void astgen::rule_2_not_greater()
{
	std::cout << "!> ";
}

void astgen::rule_2_add()
{
	std::cout << "+ ";
}

void astgen::rule_2_subtract()
{
	std::cout << "- ";
}

void astgen::rule_2_or()
{
	std::cout << "or ";
}

void astgen::rule_2_xor()
{
	std::cout << "^ ";
}

void astgen::rule_2_range()
{
	std::cout << ".. ";
}

void astgen::rule_2_multiplication()
{
	std::cout << "* ";
}

void astgen::rule_2_division()
{
	std::cout << "/ ";
}

void astgen::rule_2_modulo()
{
	std::cout << "% ";
}

void astgen::rule_2_shift_left()
{
	std::cout << "<< ";
}

void astgen::rule_2_shift_right()
{
	std::cout << ">> ";
}

void astgen::rule_2_and()
{
	std::cout << "and ";
}

void astgen::rule_1_negate()
{
	std::cout << "neg ";
}

void astgen::rule_1_complement()
{
	std::cout << "not ";
}

void astgen::rule_1_eval()
{
	std::cout << "() ";
}

void astgen::rule_1_list()
{
	std::cout << "[] ";
}

void astgen::rule_1_object()
{
	std::cout << "{} ";
}

void astgen::rule_2_subscript()
{
	std::cout << "app ";
}

void astgen::rule_2_lookup()
{
	std::cout << ". ";
}

