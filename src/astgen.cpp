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
	binop(ast::binop::equal);
}

void astgen::rule_2_lesser() {
	binop(ast::binop::lesser);
}

void astgen::rule_2_greater() {
	binop(ast::binop::greater);
}

void astgen::rule_2_not_equal() {
	binop(ast::binop::not_equal);
}

void astgen::rule_2_not_lesser() {
	binop(ast::binop::not_lesser);
}

void astgen::rule_2_not_greater() {
	binop(ast::binop::not_greater);
}

void astgen::rule_2_add() {
	binop(ast::binop::add);
}

void astgen::rule_2_subtract() {
	binop(ast::binop::subtract);
}

void astgen::rule_2_or() {
	binop(ast::binop::disjoin);
}

void astgen::rule_2_xor() {
	binop(ast::binop::exclude);
}

void astgen::rule_2_range() {
	std::cout << ".. ";
}

void astgen::rule_2_multiply() {
	binop(ast::binop::multiply);
}

void astgen::rule_2_divide() {
	binop(ast::binop::divide);
}

void astgen::rule_2_modulo() {
	binop(ast::binop::modulo);
}

void astgen::rule_2_shift_left() {
	binop(ast::binop::shift_left);
}

void astgen::rule_2_shift_right() {
	binop(ast::binop::shift_right);
}

void astgen::rule_2_and() {
	binop(ast::binop::conjoin);
}

void astgen::rule_1_negate(location loc) {
	std::cout << "neg ";
}

void astgen::rule_1_complement(location loc) {
	std::cout << "not ";
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

void astgen::binop(ast::binop::opcode id) {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::binop(id, left, right));
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


