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
	while (!ops.empty()) {
		if (expecting_term()) {
			err.parser_missing_operand(l);
			return;
		}
		commit();
	}
	out.ast_process(std::move(exp));
	assert(vals.empty());
}

void parser::token_number(location l, std::string text) {
	if (!accept_term(l)) return;
	emit(new ast::literal(ast::literal::number, text, l));
}

void parser::token_symbol(location l, std::string text) {
	if (!accept_term(l)) return;
	emit(new ast::symbol(text, l));
}

void parser::token_string(location l, std::string text) {
	if (!accept_term(l)) return;
	emit(new ast::literal(ast::literal::string, text, l));
}

void parser::token_underscore(location l) {
	if (!accept_term(l)) return;
	emit(new ast::placeholder(l));
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
		emit(new ast::join(pop(), cur()));
	}});
}

void parser::token_colon(location l) {
	infix({precedence::structure, l, [this]() {
		emit(new ast::invocation(ast::invocation::caption, pop(), cur()));
	}});
}

void parser::token_semicolon(location l) {
	infix({precedence::structure, l, [this]() {
		emit(new ast::sequence(pop(), cur()));
	}});
}

void parser::token_dot(location l) {
	infix({precedence::primary, l, [this]() {
		emit(new ast::invocation(ast::invocation::lookup, pop(), cur()));
	}});
}

void parser::token_dot_dot(location l) {
	infix({precedence::additive, l, [this]() {
		emit(new ast::range(pop(), cur()));
	}});
}

void parser::token_l_arrow(location l) {
	infix({precedence::definition, l, [this]() {
		emit(new ast::definition(ast::definition::evaluate, pop(), cur()));
	}});
}

void parser::token_r_arrow(location l) {
	infix({precedence::definition, l, [this]() {
		emit(new ast::definition(ast::definition::capture, pop(), cur()));
	}});
}

void parser::token_plus(location l) {
	infix({precedence::additive, l, [this]() {
		emit(new ast::arithmetic(ast::arithmetic::add, pop(), cur()));
	}});
}

void parser::token_hyphen(location l) {
	if (expecting_term()) {
		prefix({precedence::unary, l, [this, l]() {
			emit(new ast::invert(ast::invert::negate, cur(), l));
		}});
	} else {
		infix({precedence::additive, l, [this]() {
			emit(new ast::arithmetic(ast::arithmetic::subtract, pop(), cur()));
		}});
	}
}

void parser::token_star(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::arithmetic(ast::arithmetic::multiply, pop(), cur()));
	}});
}

void parser::token_slash(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::arithmetic(ast::arithmetic::divide, pop(), cur()));
	}});
}

void parser::token_percent(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::arithmetic(ast::arithmetic::modulo, pop(), cur()));
	}});
}

void parser::token_equal(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::relation(ast::relation::equal, pop(), cur()));
	}});
}

void parser::token_l_angle(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::relation(ast::relation::lesser, pop(), cur()));
	}});
}

void parser::token_r_angle(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::relation(ast::relation::greater, pop(), cur()));
	}});
}

void parser::token_bang(location l) {
	prefix({precedence::unary, l, [this, l]() {
		emit(new ast::invert(ast::invert::complement, cur(), l));
	}});
}

void parser::token_bang_equal(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::relation(ast::relation::not_equal, pop(), cur()));
	}});
}

void parser::token_l_bangle(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::relation(ast::relation::not_lesser, pop(), cur()));
	}});
}

void parser::token_r_bangle(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::relation(ast::relation::not_greater, pop(), cur()));
	}});
}

void parser::token_ampersand(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::logic(ast::logic::conjoin, pop(), cur()));
	}});
}

void parser::token_pipe(location l) {
	infix({precedence::additive, l, [this]() {
		emit(new ast::logic(ast::logic::disjoin, pop(), cur()));
	}});
}

void parser::token_caret(location l) {
	infix({precedence::additive, l, [this]() {
		emit(new ast::logic(ast::logic::exclude, pop(), cur()));
	}});
}

void parser::token_l_guillemet(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::arithmetic(ast::arithmetic::shift_left, pop(), cur()));
	}});
}

void parser::token_r_guillemet(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::arithmetic(ast::arithmetic::shift_right, pop(), cur()));
	}});
}

void parser::prefix(oprec op) {
	if (!accept_prefix(op.loc)) return;
	ops.push(op);
}

void parser::infix(oprec op) {
	if (!accept_infix(op.loc)) return;
	if (rightassoc(op.prec)) {
		while (!ops.empty() && op.prec < ops.top().prec) {
			commit();
		}
	} else {
		while (!ops.empty() && op.prec <= ops.top().prec) {
			commit();
		}
	}
	vals.push(std::move(exp));
	ops.push(op);
}

void parser::commit() {
	ops.top().commit();
	ops.pop();
}

bool parser::expecting_term() {
	return !exp;
}

bool parser::accept_term(location l) {
	if (expecting_term()) {
		return true;
	} else {
		err.parser_unexpected(l);
		return false;
	}
}

bool parser::accept_prefix(location l) {
	if (expecting_term()) {
		return true;
	} else {
		err.parser_unexpected(l);
		return false;
	}
}

bool parser::accept_infix(location l) {
	if (expecting_term()) {
		err.parser_missing_operand(l);
		return false;
	} else {
		return true;
	}
}

void parser::emit(ast::node *n) {
	assert(!exp);
	exp.reset(n);
}

ast::ptr parser::pop() {
	assert(!vals.empty());
	ast::ptr n = std::move(vals.top());
	vals.pop();
	return n;
}

ast::ptr parser::cur() {
	assert(exp);
	return std::move(exp);
}
