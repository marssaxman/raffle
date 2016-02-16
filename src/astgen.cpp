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
	binary(ast::binary::equal);
}

void astgen::rule_2_lesser() {
	binary(ast::binary::lesser);
}

void astgen::rule_2_greater() {
	binary(ast::binary::greater);
}

void astgen::rule_2_not_equal() {
	binary(ast::binary::not_equal);
}

void astgen::rule_2_not_lesser() {
	binary(ast::binary::not_lesser);
}

void astgen::rule_2_not_greater() {
	binary(ast::binary::not_greater);
}

void astgen::rule_2_add() {
	binary(ast::binary::add);
}

void astgen::rule_2_subtract() {
	binary(ast::binary::subtract);
}

void astgen::rule_2_or() {
	binary(ast::binary::disjoin);
}

void astgen::rule_2_xor() {
	binary(ast::binary::exclude);
}

void astgen::rule_2_range() {
	std::cout << ".. ";
}

void astgen::rule_2_multiply() {
	binary(ast::binary::multiply);
}

void astgen::rule_2_divide() {
	binary(ast::binary::divide);
}

void astgen::rule_2_modulo() {
	binary(ast::binary::modulo);
}

void astgen::rule_2_shift_left() {
	binary(ast::binary::shift_left);
}

void astgen::rule_2_shift_right() {
	binary(ast::binary::shift_right);
}

void astgen::rule_2_and() {
	binary(ast::binary::conjoin);
}

void astgen::rule_1_negate(location loc) {
	unary(ast::unary::negate, loc);
}

void astgen::rule_1_complement(location loc) {
	unary(ast::unary::complement, loc);
}

void astgen::rule_1_eval(location loc) {
	// A parenthesis group returns the value of the expression it contains, so
	// we don't need to generate an AST node which represents it. We'll simply
	// leave the current expression on top of the stack.
}

void astgen::rule_1_list(location loc) {
	unary(ast::unary::list, loc);
}

void astgen::rule_1_object(location loc) {
	unary(ast::unary::object, loc);
}

void astgen::rule_2_apply(location loc) {
	ast::node &arg = pop();
	ast::node &target = pop();
	push(new ast::subscript(ast::subscript::apply, target, arg, loc));
}

void astgen::rule_2_select(location loc) {
	ast::node &arg = pop();
	ast::node &target = pop();
	push(new ast::subscript(ast::subscript::apply, target, arg, loc));
}

void astgen::rule_2_expand(location loc) {
	ast::node &arg = pop();
	ast::node &target = pop();
	push(new ast::subscript(ast::subscript::apply, target, arg, loc));
}

void astgen::rule_2_lookup() {
	std::cout << ". ";
}

void astgen::unary(ast::unary::opcode id, location loc) {
	ast::node &source = pop();
	push(new ast::unary(id, source, loc));
}

void astgen::binary(ast::binary::opcode id) {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::binary(id, left, right));
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


