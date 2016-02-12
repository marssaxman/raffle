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

namespace op {
enum {
	// 0
	sequence = 0x00,
	// 1
	list = 0x10,
	// 2
	add = 0x20,
	subtract = 0x22,
	// 3
	multiply = 0x30,
	divide = 0x32,
	modulo = 0x34,
	// 4
	paren = 0x40,
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
	while (!states.empty() && states.top() != op::paren) {
		commit_infix();
	}
	if (!states.empty()) {
		states.pop();
	} else {
		err.parse_mismatched_paren(p);
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
		case op::list: val = out.parse_list(l, r); break;
		case op::add: val = out.parse_addition(l, r); break;
		case op::subtract: val = out.parse_subtraction(l, r); break;
		case op::multiply: val = out.parse_multiplication(l, r); break;
		case op::divide: val = out.parse_division(l, r); break;
		case op::modulo: val = out.parse_modulo(l, r); break;
	}
	values.push(val);
}
