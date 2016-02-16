// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "astgen.h"
#include <iostream>

void astgen::rule_0_empty() {
}

void astgen::rule_0_number(location loc, std::string text) {
	std::cout << text << " ";
}

void astgen::rule_0_symbol(location loc, std::string text) {
	std::cout << text << " ";
}

void astgen::rule_0_string(location loc, std::string text) {
	std::cout << text << " ";
}

void astgen::rule_0_placeholder(location loc) {
	std::cout << "_ ";
}

void astgen::rule_2_sequence() {
	std::cout << "; ";
}

void astgen::rule_2_capture() {
	std::cout << "-> ";
}

void astgen::rule_2_define() {
	std::cout << "<- ";
}

void astgen::rule_2_join() {
	std::cout << ", ";
}

void astgen::rule_2_caption() {
	std::cout << ": ";
}

void astgen::rule_2_equal() {
	infix(ast::binary::equal);
}

void astgen::rule_2_lesser() {
	infix(ast::binary::lesser);
}

void astgen::rule_2_greater() {
	infix(ast::binary::greater);
}

void astgen::rule_2_not_equal() {
	infix(ast::binary::not_equal);
}

void astgen::rule_2_not_lesser() {
	infix(ast::binary::not_lesser);
}

void astgen::rule_2_not_greater() {
	infix(ast::binary::not_greater);
}

void astgen::rule_2_add() {
	infix(ast::binary::add);
}

void astgen::rule_2_subtract() {
	infix(ast::binary::subtract);
}

void astgen::rule_2_or() {
	infix(ast::binary::disjoin);
}

void astgen::rule_2_xor() {
	infix(ast::binary::exclude);
}

void astgen::rule_2_range() {
	std::cout << ".. ";
}

void astgen::rule_2_multiply() {
	infix(ast::binary::multiply);
}

void astgen::rule_2_divide() {
	infix(ast::binary::divide);
}

void astgen::rule_2_modulo() {
	infix(ast::binary::modulo);
}

void astgen::rule_2_shift_left() {
	infix(ast::binary::shift_left);
}

void astgen::rule_2_shift_right() {
	infix(ast::binary::shift_right);
}

void astgen::rule_2_and() {
	infix(ast::binary::conjoin);
}

void astgen::rule_1_negate(location loc) {
	prefix(ast::unary::negate, loc);
}

void astgen::rule_1_complement(location loc) {
	prefix(ast::unary::complement, loc);
}

void astgen::rule_1_eval(location loc) {
	std::cout << "() ";
}

void astgen::rule_1_list(location loc) {
	std::cout << "[] ";
}

void astgen::rule_1_object(location loc) {
	std::cout << "{} ";
}

void astgen::rule_2_subscript() {
	std::cout << "app ";
}

void astgen::rule_2_lookup() {
	std::cout << ". ";
}

void astgen::infix(ast::binary::opcode id) {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::binary(id, left, right));
}

void astgen::prefix(ast::unary::opcode id, location loc) {
	ast::node &source = pop();
	push(new ast::unary(id, source, loc));
}

void astgen::push(ast::node *n) {
	val.push(output.size());
	output.emplace_back(n);
}

ast::node &astgen::pop() {
	size_t i = val.top();
	val.pop();
	return *output[i];
}


