// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "astgen.h"
#include <iostream>

using namespace ast;

void astgen::rule_0_empty() {
	push(new ast::empty);
}

void astgen::rule_0_number(location loc, std::string text) {
	push(new literal(literal::number, text, loc));
}

void astgen::rule_0_symbol(location loc, std::string text) {
	push(new symbol(text, loc));
}

void astgen::rule_0_string(location loc, std::string text) {
	push(new literal(literal::string, text, loc));
}

void astgen::rule_0_placeholder(location loc) {
	push(new ast::placeholder(loc));
}

void astgen::rule_2_sequence() {
	ast::node &next = pop();
	ast::node &exp = pop();
	push(new ast::sequence(exp, next));
}

void astgen::rule_2_capture() {
	ast::node &exp = pop();
	ast::node &sym = pop();
	push(new ast::definition(ast::definition::capture, sym, exp));
}

void astgen::rule_2_define() {
	ast::node &exp = pop();
	ast::node &sym = pop();
	push(new ast::definition(ast::definition::evaluate, sym, exp));
}

void astgen::rule_2_join() {
	ast::node &next = pop();
	ast::node &exp = pop();
	push(new ast::join(exp, next));
}

void astgen::rule_2_caption() {
	ast::node &arg = pop();
	ast::node &target = pop();
	push(new ast::invocation(ast::invocation::caption, target, arg));
}

void astgen::rule_2_equal() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::relation(ast::relation::equal, left, right));
}

void astgen::rule_2_lesser() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::relation(ast::relation::lesser, left, right));
}

void astgen::rule_2_greater() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::relation(ast::relation::greater, left, right));
}

void astgen::rule_2_not_equal() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::relation(ast::relation::not_equal, left, right));
}

void astgen::rule_2_not_lesser() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::relation(ast::relation::not_lesser, left, right));
}

void astgen::rule_2_not_greater() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::relation(ast::relation::not_greater, left, right));
}

void astgen::rule_2_add() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::arithmetic(ast::arithmetic::add, left, right));
}

void astgen::rule_2_subtract() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::arithmetic(ast::arithmetic::subtract, left, right));
}

void astgen::rule_2_or() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::logic(ast::logic::disjoin, left, right));
}

void astgen::rule_2_xor() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::logic(ast::logic::exclude, left, right));
}

void astgen::rule_2_range() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::range(left, right));
}

void astgen::rule_2_multiply() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::arithmetic(ast::arithmetic::multiply, left, right));
}

void astgen::rule_2_divide() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::arithmetic(ast::arithmetic::divide, left, right));
}

void astgen::rule_2_modulo() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::arithmetic(ast::arithmetic::modulo, left, right));
}

void astgen::rule_2_shift_left() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::arithmetic(ast::arithmetic::shift_left, left, right));
}

void astgen::rule_2_shift_right() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::arithmetic(ast::arithmetic::shift_right, left, right));
}

void astgen::rule_2_and() {
	ast::node &right = pop();
	ast::node &left = pop();
	push(new ast::logic(ast::logic::conjoin, left, right));
}

void astgen::rule_1_negate(location loc) {
	ast::node &source = pop();
	push(new ast::invert(ast::invert::negate, source, loc));
}

void astgen::rule_1_complement(location loc) {
	ast::node &source = pop();
	push(new ast::invert(ast::invert::complement, source, loc));
}

void astgen::rule_1_tuple(location loc) {
	ast::node &exp = pop();
	push(new ast::constructor(ast::constructor::tuple, exp, loc));
}

void astgen::rule_1_list(location loc) {
	ast::node &exp = pop();
	push(new ast::constructor(ast::constructor::list, exp, loc));
}

void astgen::rule_1_object(location loc) {
	ast::node &exp = pop();
	push(new ast::constructor(ast::constructor::object, exp, loc));
}

void astgen::rule_2_subscript() {
	ast::node &arg = pop();
	ast::node &target = pop();
	push(new ast::invocation(ast::invocation::subscript, target, arg));
}

void astgen::rule_2_lookup() {
	ast::node &arg = pop();
	ast::node &target = pop();
	push(new ast::invocation(ast::invocation::lookup, target, arg));
}

void astgen::push(ast::node *n) {
	output.emplace_back(n);
	val.push(*output.back());
}

ast::node &astgen::pop() {
	ast::node &out = val.top();
	val.pop();
	return out;
}


