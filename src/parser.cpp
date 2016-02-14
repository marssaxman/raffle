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
	eval = 0x00,
	list,
	object,

	// statements
	sequence = 0x10,

	// symbols
	capture = 0x30,
	define,

	// structure
	join = 0x50,
	caption,

	// relation
	equal = 0x60,
	lesser,
	greater,

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

static int precedence(int op) {
	return op >> 4;
}

static bool rightassoc(int op) {
	return precedence(op) & 1;
}

void parser::token_number(location l, std::string text) {
	term(&output::rule_number, l, text);
}

void parser::token_symbol(location l, std::string text) {
	term(&output::rule_symbol, l, text);
}

void parser::token_string(location l, std::string text) {
	term(&output::rule_string, l, text);
}

void parser::token_underscore(location l) {
	if (context != state::value) {
		accept(out.rule_placeholder());
	} else {
		err.parser_unexpected(l);
	}
}

void parser::token_paren(location l, direction d) {
	group(op::eval, &output::rule_eval, l, d);
}

void parser::token_bracket(location l, direction d) {
	group(op::list, &output::rule_list, l, d);
}

void parser::token_brace(location l, direction d) {
	group(op::object, &output::rule_object, l, d);
}

void parser::token_comma(location l) {
	binary(op::join, l);
}

void parser::token_colon(location l) {
	binary(op::caption, l);
}

void parser::token_semicolon(location l) {
	binary(op::sequence, l);
}

void parser::token_dot(location l) {
	binary(op::lookup, l);
}

void parser::token_dot_dot(location l) {
	binary(op::range, l);
}

void parser::token_arrow(location l, direction d) {
	directional(op::define, op::capture, l, d);
}

void parser::token_plus(location l) {
	binary(op::add, l);
}

void parser::token_hyphen(location l) {
	if (context == state::value) {
		binary(op::subtract, l);
	} else {
		unary(op::negate, l);
	}
}

void parser::token_star(location l) {
	binary(op::multiply, l);
}

void parser::token_slash(location l) {
	binary(op::divide, l);
}

void parser::token_percent(location l) {
	binary(op::modulo, l);
}

void parser::token_equal(location l) {
	binary(op::equal, l);
}

void parser::token_angle(location l, direction d) {
	directional(op::lesser, op::greater, l, d);
}

void parser::token_bang(location l) {
	unary(op::complement, l);
}

void parser::token_ampersand(location l) {
	binary(op::conjoin, l);
}

void parser::token_pipe(location l) {
	binary(op::disjoin, l);
}

void parser::token_caret(location l) {
	binary(op::exclude, l);
}

void parser::token_guillemet(location l, lexer::direction d) {
	directional(op::shift_left, op::shift_right, l, d);
}

void parser::flush() {
	while (!ops.empty()) {
		commit();
	}
}

void parser::accept(int val) {
	values.push(val);
	context = state::value;
}

int parser::recall() {
	int val = values.top();
	values.pop();
	return val;
}

void parser::term(int (output::*rule)(std::string), location l, std::string t)
{
	if (context != state::value) {
		accept((out.*rule)(t));
	} else {
		err.parser_unexpected(l);
	}
}

void parser::group(int tk, int (output::*rule)(int), location l, direction d) {
	switch (d) {
		case direction::left: open_group(tk, l); break;
		case direction::right: close_group(tk, rule, l); break;
	}
}

void parser::open_group(int tk, location l) {
	if (context == state::value) {
		ops.push({op::subscript, l});
	}
	ops.push({tk, l});
	context = state::empty;
}

void parser::close_group(int tk, int (output::*rule)(int), location l) {
	if (context == state::value) {
		while (!ops.empty() && ops.top().id != tk) {
			commit();
		}
	} else if (context == state::empty) {
		accept(out.rule_empty());
	} else {
		err.parser_unexpected(l);
	}
	if (!ops.empty() && ops.top().id == tk) {
		ops.pop();
		accept((out.*rule)(recall()));
	} else {
		err.parser_mismatched_group(l);
	}
}

void parser::directional(int l, int r, location loc, direction dir) {
	switch (dir) {
		case direction::left: binary(l, loc); break;
		case direction::right: binary(r, loc); break;
	}
}

void parser::unary(int tk, location l) {
	// a unary operator must precede a value, so it can only be used from
	// prefix position
	if (context == state::value) {
		err.parser_unexpected(l);
		return;
	}
	ops.push({tk, l});
}

void parser::binary(int tk, location l) {
	// a binary operator must be preceded by a value, so it cannot be used
	// from prefix position
	if (context != state::value) {
		err.parser_missing_operand(l);
		return;
	}
	int prec = precedence(tk);
	bool rightward = rightassoc(tk);
	while (!ops.empty()) {
		int prev = precedence(ops.top().id);
		if (prec > prev) break;
		if (rightward && prec == prev) break;
		commit();
	}
	ops.push({tk, l});
	context = state::infix;
}

void parser::commit() {
	token tk = ops.top();
	ops.pop();
	int v = recall();
	switch (tk.id) {
		case op::sequence: accept(out.rule_sequence(recall(), v)); break;
		case op::capture: accept(out.rule_capture(recall(), v)); break;
		case op::define: accept(out.rule_define(recall(), v)); break;
		case op::join: accept(out.rule_join(recall(), v)); break;
		case op::caption: accept(out.rule_caption(recall(), v)); break;
		case op::equal: accept(out.rule_equal(recall(), v)); break;
		case op::lesser: accept(out.rule_lesser(recall(), v)); break;
		case op::greater: accept(out.rule_greater(recall(), v)); break;
		case op::add: accept(out.rule_addition(recall(), v)); break;
		case op::subtract: accept(out.rule_subtraction(recall(), v)); break;
		case op::disjoin: accept(out.rule_or(recall(), v)); break;
		case op::exclude: accept(out.rule_xor(recall(), v)); break;
		case op::range: accept(out.rule_range(recall(), v)); break;
		case op::multiply: accept(out.rule_multiplication(recall(), v)); break;
		case op::divide: accept(out.rule_division(recall(), v)); break;
		case op::modulo: accept(out.rule_modulo(recall(), v)); break;
		case op::shift_left: accept(out.rule_shift_left(recall(), v)); break;
		case op::shift_right: accept(out.rule_shift_right(recall(), v)); break;
		case op::conjoin: accept(out.rule_and(recall(), v)); break;
		case op::negate: accept(out.rule_negate(v)); break;
		case op::complement: accept(out.rule_complement(v)); break;
		case op::subscript: accept(out.rule_subscript(recall(), v)); break;
		case op::lookup: accept(out.rule_lookup(recall(), v)); break;
		default: err.parser_unimplemented(tk.loc); break;
	}
}

