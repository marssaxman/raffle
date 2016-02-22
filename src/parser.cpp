// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"
#include <map>
#include <assert.h>

void parser::state::commit_next() {
	oprec op = ops.top();
	ops.pop();
	emit(new ast::binary(op.id, pop(), cur()));
}

bool parser::state::commit_all(location l, error &err) {
	if (expecting_term() && !ops.empty()) {
		if (ops.top().prec == precedence::structure) {
			// Trailing field delimiters are OK; we'll just ignore them, rather
			// than reopen the good old "terminators vs separators" holy war.
			ops.pop();
			exp = std::move(vals.top());
			vals.pop();
		} else {
			err.parser_missing_right_operand(l);
			emit(new ast::wildcard(l));
		}
	}
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
		case precedence::prefix:
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

void parser::state::prefix(oprec op, error &err) {
	if (expecting_term()) {
		prep(op.prec);
		ops.push(op);
		vals.emplace(new ast::wildcard(op.loc));
	} else {
		err.parser_unexpected(op.loc);
	}
}

void parser::state::infix(oprec op, error &err) {
	if (!expecting_term()) {
		prep(op.prec);
		ops.push(op);
		vals.push(std::move(exp));
	} else {
		err.parser_missing_left_operand(op.loc);
	}
}

void parser::state::term(ast::node *n) {
	if (!expecting_term()) {
		prep(precedence::primary);
		ops.push({ast::binary::apply, precedence::primary, n->loc()});
		vals.push(std::move(exp));
	}
	emit(n);
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
	if (context.commit_all(l, err)) {
		out << context.cur();
	}
}

void parser::token_number(location l, std::string text) {
	context.term(new ast::number(text, l));
}

void parser::token_identifier(location l, std::string text) {
	context.term(new ast::symbol(text, l));
}

void parser::token_string(location l, std::string text) {
	context.term(new ast::string(text, l));
}

void parser::token_underscore(location l) {
	context.term(new ast::wildcard(l));
}

void parser::token_open(location l, token::delim g) {
	outer.emplace(l, g, std::move(context));
}

void parser::token_close(location r, token::delim g) {
	if (outer.empty()) {
		err.parser_unexpected(r);
		return;
	}
	group top = std::move(outer.top());
	outer.pop();
	context.commit_all(r, err);
	// don't use cur() to build these subexpressions, because it requires that
	// there actually be a current expression, which is true for all infix and
	// prefix operators. A pair of group delimiters might not contain anything,
	// and that's a legal expression.
	ast::group *exp;
	switch (top.type) {
		case token::delim::paren:
			if (g != top.type) err.parser_expected(r, "')'", top.loc);
			exp = new ast::parens(top.loc, std::move(context.exp), r);
			break;
		case token::delim::bracket:
			if (g != top.type) err.parser_expected(r, "']'", top.loc);
			exp = new ast::brackets(top.loc, std::move(context.exp), r);
			break;
		case token::delim::brace:
			if (g != top.type) err.parser_expected(r, "'}'", top.loc);
			exp = new ast::braces(top.loc, std::move(context.exp), r);
			break;
	}
	context = std::move(top.context);
	context.term(exp);
}

void parser::token_symbol(location l, std::string text) {
	enum {
		infix = 0,
		prefix,
		flexy,
	};
	struct opdesc {
		ast::binary::opcode id;
		precedence prec;
		int mode;
	};
	static std::map<std::string, opdesc> ops = {
		{";", {ast::binary::sequence, precedence::structure}},
		{",", {ast::binary::tuple, precedence::structure}},
		{":", {ast::binary::declare, precedence::binding}},
		{":=", {ast::binary::define, precedence::binding}},
		{"::=", {ast::binary::typealias, precedence::binding}},
		{"<-", {ast::binary::assign, precedence::binding}},
		{"->", {ast::binary::capture, precedence::binding}},
		{"=", {ast::binary::eq, precedence::relation}},
		{"<", {ast::binary::lt, precedence::relation}},
		{">", {ast::binary::gt, precedence::relation}},
		{"!=", {ast::binary::neq, precedence::relation}},
		{"!<", {ast::binary::nlt, precedence::relation}},
		{"!>", {ast::binary::ngt, precedence::relation}},
		{"+", {ast::binary::add, precedence::additive}},
		{"-", {ast::binary::sub, precedence::additive, flexy}},
		{"|", {ast::binary::disjoin, precedence::additive}},
		{"^", {ast::binary::exclude, precedence::additive}},
		{"..", {ast::binary::range, precedence::additive}},
		{"!", {ast::binary::exclude, precedence::additive, prefix}},
		{"*", {ast::binary::mul, precedence::multiplicative}},
		{"/", {ast::binary::div, precedence::multiplicative}},
		{"%", {ast::binary::rem, precedence::multiplicative}},
		{"&", {ast::binary::conjoin, precedence::multiplicative}},
		{"<<", {ast::binary::shl, precedence::multiplicative}},
		{">>", {ast::binary::shr, precedence::multiplicative}},
		{".", {ast::binary::pipeline, precedence::primary}}
	};
	auto iter = ops.find(text);
	if (iter == ops.end()) {
		err.parser_unexpected(l);
		return;
	}
	opdesc &op = iter->second;
	switch(op.mode) {
		case infix:
			context.infix({op.id, op.prec, l}, err);
			break;
		case prefix:
			context.prefix({op.id, precedence::prefix, l}, err);
			break;
		case flexy:
			if (!context.expecting_term()) {
				context.infix({op.id, op.prec, l}, err);
			} else {
				context.prefix({op.id, precedence::prefix, l}, err);
			}
	}
}

