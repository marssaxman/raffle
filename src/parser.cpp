// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"

// Operator codes are grouped and spaced so that shifting right yields the
// precedence level. Furthermore, left-associative operators are even, while
// right-associative operators are odd, so masking the low bit yields the
// associativity.
// This precedence scheme is largely based on golang, since it seems generally
// simple and straightforward.

namespace op {
enum {
	// 0: statements
	sequence = 0x00,
	// 1: abstraction
	capture = 0x11,
	define = 0x13,
	// 2: structure
	list = 0x20,
	// 3: relation
	equal = 0x30,
	lesser = 0x32,
	greater = 0x34,
	// 4: add/sub
	add = 0x40,
	subtract = 0x42,
	or_ = 0x44,
	xor_ = 0x46,
	range = 0x48,
	// 5: mul/div
	multiply = 0x50,
	divide = 0x52,
	modulo = 0x54,
	shift_left = 0x56,
	shift_right = 0x58,
	and_ = 0x5A,
	// 6: unary
	negate = 0x60,
	invert = 0x62,
	complement = 0x64,
	// 7: group
	paren = 0x70,
	bracket = 0x72,
	brace = 0x74,
};
}

static int precedence(int op) { return op >> 4; }
static bool rightassoc(int op) { return op & 1; }

void parser::token_number(lexer::position p, std::string text)
{
	values.push(out.parse_number(text));
}

void parser::token_symbol(lexer::position p, std::string text)
{
	values.push(out.parse_symbol(text));
}

void parser::token_string(lexer::position p, std::string text)
{
	values.push(out.parse_string(text));
}

void parser::token_paren_open(lexer::position p)
{
	states.push(op::paren);
}

void parser::token_paren_close(lexer::position p)
{
	if (commit_group(op::paren)) {
		int val = values.top();
		values.pop();
		values.push(out.parse_paren_group(val));
	} else {
		err.parse_mismatched_paren(p);
	}
}

void parser::token_bracket_open(lexer::position p)
{
	states.push(op::bracket);
}

void parser::token_bracket_close(lexer::position p)
{
	if (commit_group(op::bracket)) {
		int val = values.top();
		values.pop();
		values.push(out.parse_bracket_group(val));
	} else {
		err.parse_mismatched_bracket(p);
	}
}

void parser::token_brace_open(lexer::position p)
{
	states.push(op::brace);
}

void parser::token_brace_close(lexer::position p)
{
	if (commit_group(op::brace)) {
		int val = values.top();
		values.pop();
		values.push(out.parse_brace_group(val));
	} else {
		err.parse_mismatched_brace(p);
	}
}

void parser::token_comma(lexer::position p)
{
	parse_infix(op::list);
}

void parser::token_semicolon(lexer::position p)
{
	parse_infix(op::sequence);
}

void parser::token_plus(lexer::position p)
{
	parse_infix(op::add);
}

void parser::token_minus(lexer::position p)
{
	parse_infix(op::subtract);
}

void parser::token_star(lexer::position p)
{
	parse_infix(op::multiply);
}

void parser::token_slash(lexer::position p)
{
	parse_infix(op::divide);
}

void parser::token_percent(lexer::position p)
{
	parse_infix(op::modulo);
}

void parser::token_lesser(lexer::position p)
{
	parse_infix(op::lesser);
}

void parser::token_equal(lexer::position p)
{
	parse_infix(op::equal);
}

void parser::token_greater(lexer::position p)
{
	parse_infix(op::greater);
}

void parser::token_and(lexer::position p)
{
	parse_infix(op::and_);
}

void parser::token_pipe(lexer::position p)
{
	parse_infix(op::or_);
}

void parser::token_bang(lexer::position p)
{
	// this is a unary prefix
}

void parser::token_tilde(lexer::position p)
{
	// this is a unary prefix
}

void parser::close()
{
	while (!states.empty()) {
		commit_infix();
	}
}

void parser::parse_infix(int tk)
{
	while (!states.empty()) {
		int prev = states.top();
		if (precedence(tk) > precedence(prev)) break;
		if (rightassoc(tk) && precedence(tk) == precedence(prev)) break;
		commit_infix();
	}
	states.push(tk);
}

void parser::commit_infix()
{
	int tk = states.top();
	states.pop();
	int r = values.top();
	values.pop();
	int l = values.top();
	values.pop();
	int val = 0;
	switch (tk) {
		case op::sequence: val = out.parse_sequence(l, r); break;
		case op::capture: val = out.parse_capture(l, r); break;
		case op::define: val = out.parse_define(l, r); break;
		case op::list: val = out.parse_list(l, r); break;
		case op::equal: val = out.parse_equal(l, r); break;
		case op::lesser: val = out.parse_lesser(l, r); break;
		case op::greater: val = out.parse_greater(l, r); break;
		case op::add: val = out.parse_addition(l, r); break;
		case op::subtract: val = out.parse_subtraction(l, r); break;
		case op::or_: val = out.parse_or(l, r); break;
		case op::xor_: val = out.parse_xor(l, r); break;
		case op::range: val = out.parse_range(l, r); break;
		case op::multiply: val = out.parse_multiplication(l, r); break;
		case op::divide: val = out.parse_division(l, r); break;
		case op::modulo: val = out.parse_modulo(l, r); break;
		case op::shift_left: val = out.parse_shift_left(l, r); break;
		case op::shift_right: val = out.parse_shift_right(l, r); break;
		case op::and_: val = out.parse_and(l, r); break;
	}
	values.push(val);
}

bool parser::commit_group(int tk)
{
	while (!states.empty() && states.top() != tk) {
		commit_infix();
	}
	if (!states.empty() && states.top() == tk) {
		states.pop();
		return true;
	} else {
		return false;
	}
}
