// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "ast.h"
#include <iostream>

using namespace ast;

int builder::make(node::type id)
{
	int i = t.size();
	t.emplace_back(new node(id));
	return i;
}

int builder::make(node::type id, std::string text)
{
	int i = t.size();
	t.emplace_back(new leaf(id, text));
	return i;
}

int builder::make(node::type id, ref source)
{
	int i = t.size();
	t.emplace_back(new twig(id, source));
	return i;
}

int builder::make(node::type id, ref left, ref right)
{
	int i = t.size();
	t.emplace_back(new branch(id, left, right));
	return i;
}

int builder::rule_number(std::string t)
{
	return make(node::number, t);
}

int builder::rule_symbol(std::string t)
{
	return make(node::symbol, t);
}

int builder::rule_string(std::string t)
{
	return make(node::string, t);
}

int builder::rule_blank()
{
	return make(node::blank);
}

int builder::rule_paren_empty()
{
	std::cout << "() ";
	return ++i;
}

int builder::rule_bracket_empty()
{
	std::cout << "[] ";
	return ++i;
}

int builder::rule_brace_empty()
{
	std::cout << "{} ";
	return ++i;
}

int builder::rule_sequence(int l, int r)
{
	std::cout << "; ";
	return ++i;
}

int builder::rule_capture(int, int)
{
	std::cout << "-> ";
	return ++i;
}

int builder::rule_define(int, int)
{
	std::cout << "<- ";
	return ++i;
}

int builder::rule_list(int l, int r)
{
	std::cout << ", ";
	return ++i;
}

int builder::rule_caption(int l, int r)
{
	std::cout << ": ";
	return ++i;
}

int builder::rule_equal(int l, int r)
{
	std::cout << "= ";
	return ++i;
}

int builder::rule_lesser(int l, int r)
{
	std::cout << "< ";
	return ++i;
}

int builder::rule_greater(int l, int r)
{
	std::cout << "> ";
	return ++i;
}

int builder::rule_not_equal(int l, int r)
{
	std::cout << "!= ";
	return ++i;
}

int builder::rule_not_lesser(int l, int r)
{
	std::cout << "!< ";
	return ++i;
}

int builder::rule_not_greater(int l, int r)
{
	std::cout << "!> ";
	return ++i;
}

int builder::rule_addition(int l, int r)
{
	std::cout << "+ ";
	return ++i;
}

int builder::rule_subtraction(int l, int r)
{
	std::cout << "- ";
	return ++i;
}

int builder::rule_or(int l, int r)
{
	std::cout << "or ";
	return ++i;
}

int builder::rule_xor(int l, int r)
{
	std::cout << "^ ";
	return ++i;
}

int builder::rule_range(int l, int r)
{
	std::cout << ".. ";
	return ++i;
}

int builder::rule_multiplication(int l, int r)
{
	std::cout << "* ";
	return ++i;
}

int builder::rule_division(int l, int r)
{
	std::cout << "/ ";
	return ++i;
}

int builder::rule_modulo(int l, int r)
{
	std::cout << "% ";
	return ++i;
}

int builder::rule_shift_left(int l, int r)
{
	std::cout << "<< ";
	return ++i;
}

int builder::rule_shift_right(int l, int r)
{
	std::cout << ">> ";
	return ++i;
}

int builder::rule_and(int l, int r)
{
	std::cout << "and ";
	return ++i;
}

int builder::rule_negate(int v)
{
	std::cout << "neg ";
	return ++i;
}

int builder::rule_complement(int v)
{
	std::cout << "not ";
	return ++i;
}

int builder::rule_paren_group(int v)
{
	std::cout << "(*) ";
	return ++i;
}

int builder::rule_bracket_group(int v)
{
	std::cout << "[*] ";
	return ++i;
}

int builder::rule_brace_group(int v)
{
	std::cout << "{*} ";
	return ++i;
}

int builder::rule_subscript(int l, int r)
{
	std::cout << "app ";
	return ++i;
}

int builder::rule_lookup(int l, int r)
{
	std::cout << ". ";
	return ++i;
}
