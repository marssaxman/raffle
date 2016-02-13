// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "ast.h"
#include <iostream>

int ast::parse_number(std::string t)
{
	std::cout << "number ";
	return ++i;
}

int ast::parse_symbol(std::string t)
{
	std::cout << "symbol ";
	return ++i;
}

int ast::parse_string(std::string t)
{
	std::cout << "string ";
	return ++i;
}

int ast::parse_paren_empty()
{
	std::cout << "() ";
	return ++i;
}

int ast::parse_bracket_empty()
{
	std::cout << "[] ";
	return ++i;
}

int ast::parse_brace_empty()
{
	std::cout << "{} ";
	return ++i;
}

int ast::parse_sequence(int l, int r)
{
	std::cout << "; ";
	return ++i;
}

int ast::parse_capture(int, int)
{
	std::cout << "-> ";
	return ++i;
}

int ast::parse_define(int, int)
{
	std::cout << "<- ";
	return ++i;
}

int ast::parse_list(int l, int r)
{
	std::cout << ", ";
	return ++i;
}

int ast::parse_caption(int l, int r)
{
	std::cout << ": ";
	return ++i;
}

int ast::parse_equal(int l, int r)
{
	std::cout << "= ";
	return ++i;
}

int ast::parse_lesser(int l, int r)
{
	return ++i;
}

int ast::parse_greater(int l, int r)
{
	return ++i;
}

int ast::parse_addition(int l, int r)
{
	return ++i;
}

int ast::parse_subtraction(int l, int r)
{
	return ++i;
}

int ast::parse_or(int l, int r)
{
	return ++i;
}

int ast::parse_xor(int l, int r)
{
	return ++i;
}

int ast::parse_range(int l, int r)
{
	return ++i;
}

int ast::parse_multiplication(int l, int r)
{
	return ++i;
}

int ast::parse_division(int l, int r)
{
	return ++i;
}

int ast::parse_modulo(int l, int r)
{
	return ++i;
}

int ast::parse_shift_left(int l, int r)
{
	return ++i;
}

int ast::parse_shift_right(int l, int r)
{
	return ++i;
}

int ast::parse_and(int l, int r)
{
	return ++i;
}

int ast::parse_negate(int v)
{
	return ++i;
}

int ast::parse_paren_group(int v)
{
	return ++i;
}

int ast::parse_bracket_group(int v)
{
	return ++i;
}

int ast::parse_brace_group(int v)
{
	return ++i;
}

int ast::parse_subscript(int l, int r)
{
	return ++i;
}
