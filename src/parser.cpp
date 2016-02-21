// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"
#include <assert.h>

void parser::token_eof(location l) {
	if (commit_all(l)) {
		out.ast_process(cur());
	}
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
	open(l, group::delim::parens);
}

void parser::token_r_paren(location r) {
	close(group::delim::parens, r);
}

void parser::token_l_bracket(location l) {
	open(l, group::delim::brackets);
}

void parser::token_r_bracket(location r) {
	close(group::delim::brackets, r);
}

void parser::token_l_brace(location l) {
	open(l, group::delim::braces);
}

void parser::token_r_brace(location r) {
	close(group::delim::braces, r);
}

void parser::token_comma(location l) {
	infix({precedence::structure, l, [this]() {
		emit(new ast::tuple(pop(), cur()));
	}});
}

void parser::token_semicolon(location l) {
	infix({precedence::structure, l, [this]() {
		emit(new ast::sequence(pop(), cur()));
	}});
}

void parser::token_colon(location l) {
	infix({precedence::binding, l, [this]() {
		emit(new ast::declare(pop(), cur()));
	}});
}

void parser::token_colon_equal(location l) {
	infix({precedence::binding, l, [this]() {
		emit(new ast::define(pop(), cur()));
	}});
}

void parser::token_double_colon_equal(location l) {
	infix({precedence::binding, l, [this]() {
		emit(new ast::typealias(pop(), cur()));
	}});
}

void parser::token_dot(location l) {
	infix({precedence::primary, l, [this]() {
		emit(new ast::pipeline(pop(), cur()));
	}});
}

void parser::token_dot_dot(location l) {
	infix({precedence::additive, l, [this]() {
		emit(new ast::range(pop(), cur()));
	}});
}

void parser::token_l_arrow(location l) {
	infix({precedence::binding, l, [this]() {
		emit(new ast::assign(pop(), cur()));
	}});
}

void parser::token_r_arrow(location l) {
	infix({precedence::binding, l, [this]() {
		emit(new ast::capture(pop(), cur()));
	}});
}

void parser::token_plus(location l) {
	infix({precedence::additive, l, [this]() {
		emit(new ast::operate(ast::operate::add, pop(), cur()));
	}});
}

void parser::token_hyphen(location l) {
	if (expecting_term()) {
		prefix({precedence::negation, l, [this, l]() {
			emit(new ast::negate(ast::negate::numeric, cur(), l));
		}});
	} else {
		infix({precedence::additive, l, [this]() {
			emit(new ast::operate(ast::operate::sub, pop(), cur()));
		}});
	}
}

void parser::token_star(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::operate(ast::operate::mul, pop(), cur()));
	}});
}

void parser::token_slash(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::operate(ast::operate::div, pop(), cur()));
	}});
}

void parser::token_percent(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::operate(ast::operate::rem, pop(), cur()));
	}});
}

void parser::token_equal(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::operate(ast::operate::eq, pop(), cur()));
	}});
}

void parser::token_tilde(location l) {
	err.parser_unexpected(l);
}

void parser::token_l_angle(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::operate(ast::operate::lt, pop(), cur()));
	}});
}

void parser::token_r_angle(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::operate(ast::operate::gt, pop(), cur()));
	}});
}

void parser::token_bang(location l) {
	prefix({precedence::negation, l, [this, l]() {
		emit(new ast::negate(ast::negate::logical, cur(), l));
	}});
}

void parser::token_bang_equal(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::operate(ast::operate::neq, pop(), cur()));
	}});
}

void parser::token_bang_tilde(location l) {
	err.parser_unexpected(l);
}

void parser::token_l_bangle(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::operate(ast::operate::nlt, pop(), cur()));
	}});
}

void parser::token_r_bangle(location l) {
	infix({precedence::relation, l, [this]() {
		emit(new ast::operate(ast::operate::ngt, pop(), cur()));
	}});
}

void parser::token_ampersand(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::operate(ast::operate::conjoin, pop(), cur()));
	}});
}

void parser::token_pipe(location l) {
	infix({precedence::additive, l, [this]() {
		emit(new ast::operate(ast::operate::disjoin, pop(), cur()));
	}});
}

void parser::token_caret(location l) {
	infix({precedence::additive, l, [this]() {
		emit(new ast::operate(ast::operate::exclude, pop(), cur()));
	}});
}

void parser::token_l_guillemet(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::operate(ast::operate::shl, pop(), cur()));
	}});
}

void parser::token_r_guillemet(location l) {
	infix({precedence::multiplicative, l, [this]() {
		emit(new ast::operate(ast::operate::shr, pop(), cur()));
	}});
}

void parser::prefix(oprec op) {
	if (!accept_prefix(op.loc)) return;
	context.ops.push(op);
}

void parser::infix(oprec op) {
	if (!accept_infix(op.loc)) return;
	// is this a right-associative operator?
	bool rightassoc = false;
	switch (op.prec) {
		case precedence::structure:
		case precedence::binding:
		case precedence::negation: rightassoc = true;
	}
	if (rightassoc) {
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

bool parser::commit_all(location l) {
	if (expecting_term() && !context.ops.empty()) {
		err.parser_missing_right_operand(l);
		emit(new ast::wildcard(l));
	}
	while (!context.ops.empty()) {
		commit_next();
	}
	assert(context.vals.empty());
	return context.exp != nullptr;
}

void parser::open(location l, group::delim g) {
	if (!accept_term(l)) return;
	outer.emplace(l, g, std::move(context));
}

void parser::close(group::delim g, location r) {
	if (outer.empty()) {
		err.parser_unexpected(r);
		return;
	}
	group top = std::move(outer.top());
	outer.pop();
	commit_all(r);
	// don't use cur() to build these subexpressions, because it requires that
	// there actually be a current expression, which is true for all infix and
	// prefix operators. A pair of group delimiters might not contain anything,
	// and that's a legal expression.
	ast::group *exp;
	switch (top.type) {
		case group::delim::parens:
			if (g != top.type) err.parser_expected(r, "')'", top.loc);
			exp = new ast::parens(top.loc, std::move(context.exp), r);
			break;
		case group::delim::brackets:
			if (g != top.type) err.parser_expected(r, "']'", top.loc);
			exp = new ast::brackets(top.loc, std::move(context.exp), r);
			break;
		case group::delim::braces:
			if (g != top.type) err.parser_expected(r, "'}'", top.loc);
			exp = new ast::braces(top.loc, std::move(context.exp), r);
			break;
	}
	context = std::move(top.context);
	emit(exp);
}

bool parser::expecting_term() {
	return !context.exp;
}

bool parser::accept_term(location l) {
	if (!expecting_term()) {
		infix({precedence::primary, l, [this](){
			emit(new ast::apply(pop(), cur()));
		}});
	}
	return true;
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
	assert(n && !context.exp);
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
