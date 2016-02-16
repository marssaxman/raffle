// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"

int parser::precedence(op x) {
	return static_cast<int>(x) >> 4;
}

bool parser::rightassoc(op x) {
	return precedence(x) & 1;
}

void parser::token_number(location l, std::string text) {
	term(&syntax::delegate::rule_0_number, l, text);
}

void parser::token_symbol(location l, std::string text) {
	term(&syntax::delegate::rule_0_symbol, l, text);
}

void parser::token_string(location l, std::string text) {
	term(&syntax::delegate::rule_0_string, l, text);
}

void parser::token_underscore(location l) {
	if (context != state::value) {
		out.rule_0_placeholder(l);
		context = state::value;
	} else {
		err.parser_unexpected(l);
	}
}

void parser::token_l_paren(location l) {
	open(op::tuple, l);
}

void parser::token_r_paren(location l) {
	close(op::tuple, &syntax::delegate::rule_1_tuple, l);
}

void parser::token_l_bracket(location l) {
	open(op::list, l);
}

void parser::token_r_bracket(location l) {
	close(op::list, &syntax::delegate::rule_1_list, l);
}

void parser::token_l_brace(location l) {
	open(op::object, l);
}

void parser::token_r_brace(location l) {
	close(op::object, &syntax::delegate::rule_1_object, l);
}

void parser::token_comma(location l) {
	infix(op::join, l);
}

void parser::token_colon(location l) {
	infix(op::caption, l);
}

void parser::token_semicolon(location l) {
	infix(op::sequence, l);
}

void parser::token_dot(location l) {
	infix(op::lookup, l);
}

void parser::token_dot_dot(location l) {
	infix(op::range, l);
}

void parser::token_l_arrow(location l) {
	infix(op::define, l);
}

void parser::token_r_arrow(location l) {
	infix(op::capture, l);
}

void parser::token_plus(location l) {
	infix(op::add, l);
}

void parser::token_hyphen(location l) {
	if (context == state::value) {
		infix(op::subtract, l);
	} else {
		prefix(op::negate, l);
	}
}

void parser::token_star(location l) {
	infix(op::multiply, l);
}

void parser::token_slash(location l) {
	infix(op::divide, l);
}

void parser::token_percent(location l) {
	infix(op::modulo, l);
}

void parser::token_equal(location l) {
	infix(op::equal, l);
}

void parser::token_l_angle(location l) {
	infix(op::lesser, l);
}

void parser::token_r_angle(location l) {
	infix(op::greater, l);
}

void parser::token_bang(location l) {
	prefix(op::complement, l);
}

void parser::token_bang_equal(location l) {
	infix(op::not_equal, l);
}

void parser::token_l_bangle(location l) {
	infix(op::not_lesser, l);
}

void parser::token_r_bangle(location l) {
	infix(op::not_greater, l);
}

void parser::token_ampersand(location l) {
	infix(op::conjoin, l);
}

void parser::token_pipe(location l) {
	infix(op::disjoin, l);
}

void parser::token_caret(location l) {
	infix(op::exclude, l);
}

void parser::token_l_guillemet(location l) {
	infix(op::shift_left, l);
}

void parser::token_r_guillemet(location l) {
	infix(op::shift_right, l);
}

void parser::flush() {
	while (!ops.empty()) {
		commit();
	}
	context = state::empty;
}

void parser::term(leaf_rule rule, location l, std::string t)
{
	if (context != state::value) {
		(out.*rule)(l, t);
		context = state::value;
	} else {
		err.parser_unexpected(l);
	}
}

void parser::open(op id, location l) {
	if (context == state::value) {
		ops.push({op::subscript, l});
	}
	ops.push({id, l});
	context = state::empty;
}

void parser::close(op id, group_rule rule, location l) {
	if (context == state::value) {
		while (!ops.empty() && ops.top().id != id) {
			commit();
		}
	} else if (context == state::empty) {
		out.rule_0_empty();
	} else {
		err.parser_unexpected(l);
	}
	if (!ops.empty() && ops.top().id == id) {
		l = ops.top().loc + l;
		ops.pop();
		(out.*(rule))(l);
		context = state::value;
	} else {
		err.parser_mismatched_group(l);
	}
}

void parser::prefix(op id, location l) {
	// a unary operator must precede a value, so it can only be used from
	// prefix position
	if (context == state::value) {
		err.parser_unexpected(l);
		return;
	}
	ops.push({id, l});
	context = state::prefix;
}

void parser::infix(op id, location l) {
	// a binary operator must be preceded by a value, so it cannot be used
	// from prefix position
	if (context != state::value) {
		err.parser_missing_operand(l);
		return;
	}
	int prec = precedence(id);
	bool rightward = rightassoc(id);
	while (!ops.empty()) {
		int prev = precedence(ops.top().id);
		if (prec > prev) break;
		if (rightward && prec == prev) break;
		commit();
	}
	ops.push({id, l});
	context = state::infix;
}

void parser::commit() {
	switch (ops.top().id) {
		case op::sequence: out.rule_2_sequence(); break;
		case op::capture: out.rule_2_capture(); break;
		case op::define: out.rule_2_define(); break;
		case op::join: out.rule_2_join(); break;
		case op::caption: out.rule_2_caption(); break;
		case op::equal: out.rule_2_equal(); break;
		case op::lesser: out.rule_2_lesser(); break;
		case op::greater: out.rule_2_greater(); break;
		case op::not_equal: out.rule_2_not_equal(); break;
		case op::not_lesser: out.rule_2_not_lesser(); break;
		case op::not_greater: out.rule_2_not_greater(); break;
		case op::add: out.rule_2_add(); break;
		case op::subtract: out.rule_2_subtract(); break;
		case op::disjoin: out.rule_2_or(); break;
		case op::exclude: out.rule_2_xor(); break;
		case op::range: out.rule_2_range(); break;
		case op::multiply: out.rule_2_multiply(); break;
		case op::divide: out.rule_2_divide(); break;
		case op::modulo: out.rule_2_modulo(); break;
		case op::shift_left: out.rule_2_shift_left(); break;
		case op::shift_right: out.rule_2_shift_right(); break;
		case op::conjoin: out.rule_2_and(); break;
		case op::negate: out.rule_1_negate(ops.top().loc); break;
		case op::complement: out.rule_1_complement(ops.top().loc); break;
		case op::subscript: out.rule_2_subscript(); break;
		case op::lookup: out.rule_2_lookup(); break;
		default: err.parser_unimplemented(ops.top().loc); break;
	}
	ops.pop();
	context = state::value;
}

