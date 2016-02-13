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
	caption = 0x22,
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
	subscript = 0x62,
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
	accept(out.parse_number(text));
}

void parser::token_symbol(lexer::position p, std::string text)
{
	accept(out.parse_symbol(text));
}

void parser::token_string(lexer::position p, std::string text)
{
	accept(out.parse_string(text));
}

void parser::token_paren_empty(lexer::position p)
{
	if (!prefix) {
		states.push(op::subscript);
	}
	accept(out.parse_paren_empty());
}

void parser::token_paren_open(lexer::position p)
{
	if (!prefix) {
		states.push(op::subscript);
	}
	states.push(op::paren);
	prefix = true;
}

void parser::token_paren_close(lexer::position p)
{
	if (close_group(op::paren)) {
		accept(out.parse_paren_group(recall()));
	} else {
		err.parse_mismatched_paren(p);
	}
	prefix = false;
}

void parser::token_bracket_empty(lexer::position p)
{
	if (!prefix) {
		states.push(op::subscript);
	}
	accept(out.parse_bracket_empty());
}

void parser::token_bracket_open(lexer::position p)
{
	if (!prefix) {
		states.push(op::subscript);
	}
	states.push(op::bracket);
	prefix = true;
}

void parser::token_bracket_close(lexer::position p)
{
	if (close_group(op::bracket)) {
		accept(out.parse_bracket_group(recall()));
	} else {
		err.parse_mismatched_bracket(p);
	}
	prefix = false;
}

void parser::token_brace_empty(lexer::position p)
{
	if (!prefix) {
		states.push(op::subscript);
	}
	accept(out.parse_brace_empty());
}

void parser::token_brace_open(lexer::position p)
{
	if (!prefix) {
		states.push(op::subscript);
	}
	states.push(op::brace);
	prefix = true;
}

void parser::token_brace_close(lexer::position p)
{
	if (close_group(op::brace)) {
		accept(out.parse_brace_group(recall()));
	} else {
		err.parse_mismatched_brace(p);
	}
	prefix = false;
}

void parser::token_comma(lexer::position p)
{
	parse_infix(op::list);
}

void parser::token_colon(lexer::position p)
{
	parse_infix(op::caption);
}

void parser::token_semicolon(lexer::position p)
{
	parse_infix(op::sequence);
}

void parser::token_arrow_left(lexer::position p)
{
	parse_infix(op::define);
}

void parser::token_arrow_right(lexer::position p)
{
	parse_infix(op::capture);
}

void parser::token_plus(lexer::position p)
{
	parse_infix(op::add);
}

void parser::token_hyphen(lexer::position p)
{
	if (prefix) {
		parse_prefix(op::negate);
	} else {
		parse_infix(op::subtract);
	}
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

void parser::token_equal(lexer::position p)
{
	parse_infix(op::equal);
}

void parser::token_diamond(lexer::position p)
{
	err.parse_unexpected(p);
}

void parser::token_angle_left(lexer::position p)
{
	parse_infix(op::lesser);
}

void parser::token_angle_right(lexer::position p)
{
	parse_infix(op::greater);
}

void parser::token_ampersand(lexer::position p)
{
	parse_infix(op::and_);
}

void parser::token_pipe(lexer::position p)
{
	parse_infix(op::or_);
}

void parser::token_caret(lexer::position p)
{
	parse_infix(op::xor_);
}

void parser::token_shift_left(lexer::position p)
{
	parse_infix(op::shift_left);
}

void parser::token_shift_right(lexer::position p)
{
	parse_infix(op::shift_right);
}

void parser::flush()
{
	while (!states.empty()) {
		commit_op();
	}
}

void parser::accept(int val)
{
	values.push(val);
	prefix = false;
}

int parser::recall()
{
	int val = values.top();
	values.pop();
	return val;
}

void parser::parse_prefix(int tk)
{
	states.push(tk);
}

void parser::parse_infix(int tk)
{
	while (!states.empty()) {
		int prev = states.top();
		if (precedence(tk) > precedence(prev)) break;
		if (rightassoc(tk) && precedence(tk) == precedence(prev)) break;
		commit_op();
	}
	states.push(tk);
	prefix = true;
}

void parser::commit_op()
{
	int tk = states.top();
	states.pop();
	int v = recall();
	switch (tk) {
		case op::sequence: accept(out.parse_sequence(recall(), v)); break;
		case op::capture: accept(out.parse_capture(recall(), v)); break;
		case op::define: accept(out.parse_define(recall(), v)); break;
		case op::list: accept(out.parse_list(recall(), v)); break;
		case op::caption: accept(out.parse_caption(recall(), v)); break;
		case op::equal: accept(out.parse_equal(recall(), v)); break;
		case op::lesser: accept(out.parse_lesser(recall(), v)); break;
		case op::greater: accept(out.parse_greater(recall(), v)); break;
		case op::add: accept(out.parse_addition(recall(), v)); break;
		case op::subtract: accept(out.parse_subtraction(recall(), v)); break;
		case op::or_: accept(out.parse_or(recall(), v)); break;
		case op::xor_: accept(out.parse_xor(recall(), v)); break;
		case op::range: accept(out.parse_range(recall(), v)); break;
		case op::multiply: accept(out.parse_multiplication(recall(), v)); break;
		case op::divide: accept(out.parse_division(recall(), v)); break;
		case op::modulo: accept(out.parse_modulo(recall(), v)); break;
		case op::shift_left: accept(out.parse_shift_left(recall(), v)); break;
		case op::shift_right: accept(out.parse_shift_right(recall(), v)); break;
		case op::and_: accept(out.parse_and(recall(), v)); break;
		case op::negate: accept(out.parse_negate(v)); break;
	}
}

bool parser::close_group(int tk)
{
	while (!states.empty()) {
		if (states.top() == tk) {
			states.pop();
			return true;
		}
		commit_op();
	}
	return false;
}
