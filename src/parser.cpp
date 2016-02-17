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
		case precedence::structure: return true;
		case precedence::relation: return false;
		case precedence::additive: return false;
		case precedence::multiplicative: return false;
		case precedence::unary: return true;
		case precedence::compose: return false;
		case precedence::primary: return false;
	}
}

void parser::token_eof(location l) {
	if (!accept_delim(l, state::delim::file)) return;
	if (context.exp) {
		out.ast_process(cur());
		context.exp.reset();
	}
	out.ast_done();
}

void parser::token_number(location l, std::string text) {
	if (!accept_term(l)) return;
	emit(new ast::number(text, l));
}

void parser::token_symbol(location l, std::string text) {
	if (!accept_term(l)) return;
	emit(new ast::symbol(text, l));
}

void parser::token_string(location l, std::string text) {
	if (!accept_term(l)) return;
	emit(new ast::string(text, l));
}

void parser::token_underscore(location l) {
	if (!accept_term(l)) return;
	emit(new ast::wildcard(l));
}

void parser::token_l_paren(location l) {
	open(l, state::delim::paren);
}

void parser::token_r_paren(location r) {
	if (!accept_delim(r, state::delim::paren)) return;
	close(ast::constructor::tuple, r);
}

void parser::token_l_bracket(location l) {
	open(l, state::delim::bracket);
}

void parser::token_r_bracket(location r) {
	if (!accept_delim(r, state::delim::bracket)) return;
	close(ast::constructor::list, r);
}

void parser::token_l_brace(location l) {
	open(l, state::delim::brace);
}

void parser::token_r_brace(location r) {
	if (!accept_delim(r, state::delim::brace)) return;
	close(ast::constructor::object, r);
}

void parser::token_comma(location l) {
	infix({precedence::structure, l, [this]() {
		emit(new ast::tuple(pop(), cur()));
	}});
}

void parser::token_colon(location l) {
	infix({precedence::primary, l, [this]() {
		emit(new ast::define(pop(), cur()));
	}});
}

void parser::token_semicolon(location l) {
	if (!accept_infix(l)) return;
	commit_all(l);
	if (context.grouping == state::delim::file) {
		// Top-level statements should be reported to the delegate.
		out.ast_process(cur());
		context.exp.reset();
	} else {
		// Statements in nested scopes are queued for later use.
		context.items.push_back(cur());
	}
}

void parser::token_dot(location l) {
	infix({precedence::compose, l, [this]() {
		emit(new ast::compose(pop(), cur()));
	}});
}

void parser::token_dot_dot(location l) {
	infix({precedence::additive, l, [this]() {
		emit(new ast::range(pop(), cur()));
	}});
}

void parser::token_l_arrow(location l) {
	infix({precedence::statement, l, [this]() {
		emit(new ast::assign(pop(), cur()));
	}});
}

void parser::token_r_arrow(location l) {
	infix({precedence::statement, l, [this]() {
		emit(new ast::capture(pop(), cur()));
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
	context.ops.push(op);
}

void parser::infix(oprec op) {
	if (!accept_infix(op.loc)) return;
	if (rightassoc(op.prec)) {
		while (!context.ops.empty() && op.prec < context.ops.top().prec) {
			commit_next();
		}
	} else {
		while (!context.ops.empty() && op.prec <= context.ops.top().prec) {
			commit_next();
		}
	}
	context.vals.push(std::move(context.exp));
	context.ops.push(op);
}

void parser::commit_next() {
	context.ops.top().commit();
	context.ops.pop();
}

void parser::commit_all(location l) {
	if (expecting_term() && !context.ops.empty()) {
		err.parser_missing_right_operand(l);
		emit(new ast::wildcard(l));
	}
	while (!context.ops.empty()) {
		commit_next();
	}
	assert(context.vals.empty());
}

void parser::open(location l, state::delim g) {
	if (!expecting_term()) {
		infix({precedence::primary, l, [this](){
			emit(new ast::apply(pop(), cur()));
		}});
	}
	outer.push(std::move(context));
	context.startloc = l;
	context.grouping = g;
}

void parser::close(ast::constructor::opcode c, location r) {
	if (context.exp) {
		context.items.push_back(std::move(context.exp));
	}
	std::list<ast::ptr> result = std::move(context.items);
	location l = context.startloc + r;
	context = std::move(outer.top());
	outer.pop();
	assert(!context.exp);
	emit(new ast::constructor(c, std::move(result), l));
}

bool parser::accept_delim(location l, state::delim g) {
	if (context.grouping != g) {
		err.parser_mismatched_group(l);
		return false;
	}
	commit_all(l);
	return true;
}

bool parser::expecting_term() {
	return !context.exp;
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
		err.parser_missing_left_operand(l);
		return false;
	} else {
		return true;
	}
}

void parser::emit(ast::node *n) {
	assert(!context.exp);
	context.exp.reset(n);
}

ast::ptr parser::pop() {
	assert(!context.vals.empty());
	ast::ptr n = std::move(context.vals.top());
	context.vals.pop();
	return n;
}

ast::ptr parser::cur() {
	assert(context.exp);
	return std::move(context.exp);
}
