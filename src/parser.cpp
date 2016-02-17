// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"
#include <assert.h>

bool parser::rightassoc(precedence x) {
	switch (x) {
		case precedence::statement: return true;
		case precedence::definition: return true;
		case precedence::structure: return true;
		case precedence::relation: return false;
		case precedence::additive: return false;
		case precedence::multiplicative: return false;
		case precedence::unary: return true;
		case precedence::primary: return false;
	}
}

void parser::token_eof(location l) {
	if (expecting_term) {
		err.parser_missing_operand(l);
		return;
	}
	while (!ops.empty()) {
		commit();
	}
	if (vals.empty()) return;
	out.ast_process(pop());
	assert(vals.empty());
}

void parser::token_number(location l, std::string text) {
	if (!accept_term(l)) return;
	push(new ast::literal(ast::literal::number, text, l));
}

void parser::token_symbol(location l, std::string text) {
	if (!accept_term(l)) return;
	push(new ast::symbol(text, l));
}

void parser::token_string(location l, std::string text) {
	if (!accept_term(l)) return;
	push(new ast::literal(ast::literal::string, text, l));
}

void parser::token_underscore(location l) {
	if (!accept_term(l)) return;
	push(new ast::placeholder(l));
}

void parser::token_l_paren(location l) {
	err.parser_unexpected(l);
}

void parser::token_r_paren(location l) {
	err.parser_unexpected(l);
}

void parser::token_l_bracket(location l) {
	err.parser_unexpected(l);
}

void parser::token_r_bracket(location l) {
	err.parser_unexpected(l);
}

void parser::token_l_brace(location l) {
	err.parser_unexpected(l);
}

void parser::token_r_brace(location l) {
	err.parser_unexpected(l);
}

void parser::token_comma(location l) {
	infix({precedence::structure, l, [this]() {
		push(new ast::join(pop(), pop()));
	}});
}

void parser::token_colon(location l) {
	infix({precedence::structure, l, [this]() {
		push(new ast::invocation(ast::invocation::caption, pop(), pop()));
	}});
}

void parser::token_semicolon(location l) {
	infix({precedence::structure, l, [this]() {
		push(new ast::sequence(pop(), pop()));
	}});
}

void parser::token_dot(location l) {
	infix({precedence::primary, l, [this]() {
		push(new ast::invocation(ast::invocation::lookup, pop(), pop()));
	}});
}

void parser::token_dot_dot(location l) {
	infix({precedence::additive, l, [this]() {
		push(new ast::range(pop(), pop()));
	}});
}

void parser::token_l_arrow(location l) {
	infix({precedence::definition, l, [this]() {
		push(new ast::definition(ast::definition::evaluate, pop(), pop()));
	}});
}

void parser::token_r_arrow(location l) {
	infix({precedence::definition, l, [this]() {
		push(new ast::definition(ast::definition::capture, pop(), pop()));
	}});
}

void parser::token_plus(location l) {
	infix({precedence::additive, l, [this]() {
		push(new ast::arithmetic(ast::arithmetic::add, pop(), pop()));
	}});
}

void parser::token_hyphen(location l) {
	if (expecting_term) {
		prefix({precedence::unary, l, [this, l]() {
			push(new ast::invert(ast::invert::negate, pop(), l));
		}});
	} else {
		infix({precedence::additive, l, [this]() {
			push(new ast::arithmetic(ast::arithmetic::subtract, pop(), pop()));
		}});
	}
}

void parser::token_star(location l) {
	infix({precedence::multiplicative, l, [this]() {
		push(new ast::arithmetic(ast::arithmetic::multiply, pop(), pop()));
	}});
}

void parser::token_slash(location l) {
	infix({precedence::multiplicative, l, [this]() {
		push(new ast::arithmetic(ast::arithmetic::divide, pop(), pop()));
	}});
}

void parser::token_percent(location l) {
	infix({precedence::multiplicative, l, [this]() {
		push(new ast::arithmetic(ast::arithmetic::modulo, pop(), pop()));
	}});
}

void parser::token_equal(location l) {
	infix({precedence::relation, l, [this]() {
		push(new ast::relation(ast::relation::equal, pop(), pop()));
	}});
}

void parser::token_l_angle(location l) {
	infix({precedence::relation, l, [this]() {
		push(new ast::relation(ast::relation::lesser, pop(), pop()));
	}});
}

void parser::token_r_angle(location l) {
	infix({precedence::relation, l, [this]() {
		push(new ast::relation(ast::relation::greater, pop(), pop()));
	}});
}

void parser::token_bang(location l) {
	prefix({precedence::unary, l, [this, l]() {
		push(new ast::invert(ast::invert::complement, pop(), l));
	}});
}

void parser::token_bang_equal(location l) {
	infix({precedence::relation, l, [this]() {
		push(new ast::relation(ast::relation::not_equal, pop(), pop()));
	}});
}

void parser::token_l_bangle(location l) {
	infix({precedence::relation, l, [this]() {
		push(new ast::relation(ast::relation::not_lesser, pop(), pop()));
	}});
}

void parser::token_r_bangle(location l) {
	infix({precedence::relation, l, [this]() {
		push(new ast::relation(ast::relation::not_greater, pop(), pop()));
	}});
}

void parser::token_ampersand(location l) {
	infix({precedence::multiplicative, l, [this]() {
		push(new ast::logic(ast::logic::conjoin, pop(), pop()));
	}});
}

void parser::token_pipe(location l) {
	infix({precedence::additive, l, [this]() {
		push(new ast::logic(ast::logic::disjoin, pop(), pop()));
	}});
}

void parser::token_caret(location l) {
	infix({precedence::additive, l, [this]() {
		push(new ast::logic(ast::logic::exclude, pop(), pop()));
	}});
}

void parser::token_l_guillemet(location l) {
	infix({precedence::multiplicative, l, [this]() {
		push(new ast::arithmetic(ast::arithmetic::shift_left, pop(), pop()));
	}});
}

void parser::token_r_guillemet(location l) {
	infix({precedence::multiplicative, l, [this]() {
		push(new ast::arithmetic(ast::arithmetic::shift_right, pop(), pop()));
	}});
}

void parser::prefix(oprec op) {
	if (!accept_prefix(op.loc)) return;
	ops.push(op);
}

void parser::infix(oprec op) {
	if (!accept_infix(op.loc)) return;
	if (rightassoc(op.prec)) {
		while (!ops.empty() && ops.top().prec < op.prec) {
			commit();
		}
	} else {
		while (!ops.empty() && ops.top().prec <= op.prec) {
			commit();
		}
	}
	ops.push(op);
}

void parser::commit() {
	ops.top().commit();
	ops.pop();
}

bool parser::accept_term(location l) {
	if (expecting_term) {
		expecting_term = false;
		return true;
	} else {
		err.parser_unexpected(l);
		return false;
	}
}

bool parser::accept_prefix(location l) {
	if (expecting_term) {
		return true;
	} else {
		err.parser_unexpected(l);
		return false;
	}
}

bool parser::accept_infix(location l) {
	if (expecting_term) {
		err.parser_missing_operand(l);
		return false;
	} else {
		expecting_term = true;
		return true;
	}
}

void parser::push(ast::node *n) {
	vals.emplace(n);
}

ast::ptr parser::pop() {
	assert(!vals.empty());
	ast::ptr n = std::move(vals.front());
	vals.pop();
	return n;
}

