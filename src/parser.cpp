// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"
#include <assert.h>

void parser::state::commit_next() {
	oprec op = ops.top();
	ops.pop();
	emit(new ast::binary(op.id, pop(), cur()));
}

bool parser::state::commit_all() {
	while (!ops.empty()) {
		commit_next();
	}
	assert(vals.empty());
	return exp != nullptr;
}

bool parser::state::expecting_term() {
	return !exp;
}

void parser::state::prep(precedence prec) {
	switch (prec) {
		case precedence::binding:
		case precedence::negation:
			// right-associative
			while (!ops.empty() && prec < ops.top().prec) {
				commit_next();
			}
			break;
		default:
			// left-associative
			while (!ops.empty() && prec <= ops.top().prec) {
				commit_next();
			}
			break;
	}
}

void parser::state::push_unary(oprec op, error &err) {
	if (expecting_term()) {
		prep(op.prec);
		ops.push(op);
		vals.emplace(new ast::wildcard(op.loc));
	} else {
		err.parser_unexpected(op.loc);
	}
}

void parser::state::push_binary(oprec op, error &err) {
	if (!expecting_term()) {
		prep(op.prec);
		ops.push(op);
		vals.push(std::move(exp));
	} else {
		err.parser_missing_left_operand(op.loc);
	}
}

void parser::state::emit(ast::node *n) {
	assert(n && !exp);
	exp.reset(n);
}

ast::ptr parser::state::pop() {
	assert(!vals.empty());
	ast::ptr n = std::move(vals.top());
	vals.pop();
	return n;
}

ast::ptr parser::state::cur() {
	assert(exp);
	return std::move(exp);
}

void parser::token_eof(location l) {
	if (commit_all(l)) {
		out << cur();
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
	infix({ast::binary::tuple, precedence::structure, l});
}

void parser::token_semicolon(location l) {
	infix({ast::binary::sequence, precedence::structure, l});
}

void parser::token_colon(location l) {
	infix({ast::binary::declare, precedence::binding, l});
}

void parser::token_colon_equal(location l) {
	infix({ast::binary::define, precedence::binding, l});
}

void parser::token_double_colon_equal(location l) {
	infix({ast::binary::typealias, precedence::binding, l});
}

void parser::token_dot(location l) {
	infix({ast::binary::pipeline, precedence::primary, l});
}

void parser::token_dot_dot(location l) {
	infix({ast::binary::range, precedence::additive, l});
}

void parser::token_l_arrow(location l) {
	infix({ast::binary::assign, precedence::binding, l});
}

void parser::token_r_arrow(location l) {
	infix({ast::binary::capture, precedence::binding, l});
}

void parser::token_plus(location l) {
	infix({ast::binary::add, precedence::additive, l});
}

void parser::token_hyphen(location l) {
	if (context.expecting_term()) {
		prefix({ast::binary::sub, precedence::negation, l});
	} else {
		infix({ast::binary::sub, precedence::additive, l});
	}
}

void parser::token_star(location l) {
	infix({ast::binary::mul, precedence::multiplicative, l});
}

void parser::token_slash(location l) {
	infix({ast::binary::div, precedence::multiplicative, l});
}

void parser::token_percent(location l) {
	infix({ast::binary::rem, precedence::multiplicative, l});
}

void parser::token_equal(location l) {
	infix({ast::binary::eq, precedence::relation, l});
}

void parser::token_tilde(location l) {
	err.parser_unexpected(l);
}

void parser::token_l_angle(location l) {
	infix({ast::binary::lt, precedence::relation, l});
}

void parser::token_r_angle(location l) {
	infix({ast::binary::gt, precedence::relation, l});
}

void parser::token_bang(location l) {
	prefix({ast::binary::sub, precedence::negation, l});
}

void parser::token_bang_equal(location l) {
	infix({ast::binary::neq, precedence::relation, l});
}

void parser::token_bang_tilde(location l) {
	err.parser_unexpected(l);
}

void parser::token_l_bangle(location l) {
	infix({ast::binary::nlt, precedence::relation, l});
}

void parser::token_r_bangle(location l) {
	infix({ast::binary::ngt, precedence::relation, l});
}

void parser::token_ampersand(location l) {
	infix({ast::binary::conjoin, precedence::multiplicative, l});
}

void parser::token_pipe(location l) {
	infix({ast::binary::disjoin, precedence::additive, l});
}

void parser::token_caret(location l) {
	infix({ast::binary::exclude, precedence::additive, l});
}

void parser::token_l_guillemet(location l) {
	infix({ast::binary::shl, precedence::multiplicative, l});
}

void parser::token_r_guillemet(location l) {
	infix({ast::binary::shr, precedence::multiplicative, l});
}

void parser::prefix(oprec op) {
	context.push_unary(op, err);
}

void parser::infix(oprec op) {
	context.push_binary(op, err);
}

bool parser::commit_all(location l) {
	if (context.expecting_term() && !context.ops.empty()) {
		if (context.ops.top().prec == precedence::structure) {
			// Trailing field delimiters are OK; we'll just ignore them, rather
			// than reopen the good old "terminators vs separators" holy war.
			context.ops.pop();
			context.exp = std::move(context.vals.top());
			context.vals.pop();
		} else {
			err.parser_missing_right_operand(l);
			emit(new ast::wildcard(l));
		}
	}
	context.commit_all();
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

bool parser::accept_term(location l) {
	if (!context.expecting_term()) {
		infix({ast::binary::apply, precedence::primary, l});
	}
	return true;
}

