// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"

// Operators are grouped into precedence levels such that shifting the code
// right by 4 yields the precedence group number. Even-numbered groups have
// left associativity and odd-numbered groups have right associativity.


namespace op {
enum {
	// groups
	paren = 0x00,
	bracket,
	brace,

	// statements
	sequence = 0x10,

	// symbols
	capture = 0x30,
	define,

	// structure
	list = 0x50,
	caption,

	// relation
	equal = 0x60,
	lesser,
	greater,
	not_equal,
	not_lesser,
	not_greater,

	// additive
	add = 0x80,
	subtract,
	disjoin,
	exclude,
	range,

	// multiplicative
	multiply = 0xA0,
	divide,
	modulo,
	shift_left,
	shift_right,
	conjoin,

	// unary
	negate = 0xB0,
	complement,

	// primary
	lookup = 0xC0,
	subscript,
};
}

static int precedence(int op) { return op >> 4; }
static bool rightassoc(int op) { return precedence(op) & 1; }

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
		states.push({op::subscript, p});
	}
	accept(out.parse_paren_empty());
}

void parser::token_paren_open(lexer::position p)
{
	if (!prefix) {
		states.push({op::subscript, p});
	}
	states.push({op::paren, p});
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
		states.push({op::subscript, p});
	}
	accept(out.parse_bracket_empty());
}

void parser::token_bracket_open(lexer::position p)
{
	if (!prefix) {
		states.push({op::subscript, p});
	}
	states.push({op::bracket, p});
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
		states.push({op::subscript, p});
	}
	accept(out.parse_brace_empty());
}

void parser::token_brace_open(lexer::position p)
{
	if (!prefix) {
		states.push({op::subscript, p});
	}
	states.push({op::brace, p});
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
	parse_infix(op::list, p);
}

void parser::token_colon(lexer::position p)
{
	parse_infix(op::caption, p);
}

void parser::token_semicolon(lexer::position p)
{
	parse_infix(op::sequence, p);
}

void parser::token_dot(lexer::position p)
{
	parse_infix(op::lookup, p);
}

void parser::token_dot_dot(lexer::position p)
{
	parse_infix(op::range, p);
}

void parser::token_arrow_left(lexer::position p)
{
	parse_infix(op::define, p);
}

void parser::token_arrow_right(lexer::position p)
{
	parse_infix(op::capture, p);
}

void parser::token_plus(lexer::position p)
{
	parse_infix(op::add, p);
}

void parser::token_hyphen(lexer::position p)
{
	if (prefix) {
		parse_prefix(op::negate, p);
	} else {
		parse_infix(op::subtract, p);
	}
}

void parser::token_star(lexer::position p)
{
	parse_infix(op::multiply, p);
}

void parser::token_slash(lexer::position p)
{
	parse_infix(op::divide, p);
}

void parser::token_percent(lexer::position p)
{
	parse_infix(op::modulo, p);
}

void parser::token_equal(lexer::position p)
{
	parse_infix(op::equal, p);
}

void parser::token_lesser(lexer::position p)
{
	parse_infix(op::lesser, p);
}

void parser::token_greater(lexer::position p)
{
	parse_infix(op::greater, p);
}

void parser::token_bang_equal(lexer::position p)
{
	parse_infix(op::not_equal, p);
}

void parser::token_bang_lesser(lexer::position p)
{
	parse_infix(op::not_lesser, p);
}

void parser::token_bang_greater(lexer::position p)
{
	parse_infix(op::not_greater, p);
}

void parser::token_bang(lexer::position p)
{
	parse_prefix(op::complement, p);
}

void parser::token_ampersand(lexer::position p)
{
	parse_infix(op::conjoin, p);
}

void parser::token_pipe(lexer::position p)
{
	parse_infix(op::disjoin, p);
}

void parser::token_caret(lexer::position p)
{
	parse_infix(op::exclude, p);
}

void parser::token_shift_left(lexer::position p)
{
	parse_infix(op::shift_left, p);
}

void parser::token_shift_right(lexer::position p)
{
	parse_infix(op::shift_right, p);
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

void parser::parse_prefix(int tk, lexer::position p)
{
	states.push({tk, p});
}

void parser::parse_infix(int tk, lexer::position p)
{
	while (!states.empty()) {
		int prev = states.top().id;
		if (precedence(tk) > precedence(prev)) break;
		if (rightassoc(tk) && precedence(tk) == precedence(prev)) break;
		commit_op();
	}
	states.push({tk, p});
	prefix = true;
}

void parser::commit_op()
{
	token tk = states.top();
	states.pop();
	int v = recall();
	switch (tk.id) {
		case op::sequence: accept(out.parse_sequence(recall(), v)); break;
		case op::capture: accept(out.parse_capture(recall(), v)); break;
		case op::define: accept(out.parse_define(recall(), v)); break;
		case op::list: accept(out.parse_list(recall(), v)); break;
		case op::caption: accept(out.parse_caption(recall(), v)); break;
		case op::equal: accept(out.parse_equal(recall(), v)); break;
		case op::lesser: accept(out.parse_lesser(recall(), v)); break;
		case op::greater: accept(out.parse_greater(recall(), v)); break;
		case op::not_equal: accept(out.parse_not_equal(recall(), v)); break;
		case op::not_lesser: accept(out.parse_not_lesser(recall(), v)); break;
		case op::not_greater: accept(out.parse_not_greater(recall(), v)); break;
		case op::add: accept(out.parse_addition(recall(), v)); break;
		case op::subtract: accept(out.parse_subtraction(recall(), v)); break;
		case op::disjoin: accept(out.parse_or(recall(), v)); break;
		case op::exclude: accept(out.parse_xor(recall(), v)); break;
		case op::range: accept(out.parse_range(recall(), v)); break;
		case op::multiply: accept(out.parse_multiplication(recall(), v)); break;
		case op::divide: accept(out.parse_division(recall(), v)); break;
		case op::modulo: accept(out.parse_modulo(recall(), v)); break;
		case op::shift_left: accept(out.parse_shift_left(recall(), v)); break;
		case op::shift_right: accept(out.parse_shift_right(recall(), v)); break;
		case op::conjoin: accept(out.parse_and(recall(), v)); break;
		case op::negate: accept(out.parse_negate(v)); break;
		case op::complement: accept(out.parse_complement(v)); break;
		case op::subscript: accept(out.parse_subscript(recall(), v)); break;
		case op::lookup: accept(out.parse_lookup(recall(), v)); break;
		default: err.parse_unimplemented(tk.pos); break;
	}
}

bool parser::close_group(int tk)
{
	while (!states.empty()) {
		if (states.top().id == tk) {
			states.pop();
			return true;
		}
		commit_op();
	}
	return false;
}
