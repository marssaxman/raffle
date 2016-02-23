// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"
#include <map>
#include <assert.h>

void parser::token_eof(location loc) {
	
}

void parser::token_number(location loc, std::string text) {
	term(ast::leaf::number, text, loc);
}

void parser::token_identifier(location loc, std::string text) {
	out.build_leaf(ast::leaf::symbol, text, loc);
}

void parser::token_string(location loc, std::string text) {
	out.build_leaf(ast::leaf::string, text, loc);
}

void parser::token_underscore(location loc) {
	out.build_blank(loc);
}

void parser::token_open(location loc, token::delim c) {
}

void parser::token_close(location loc, token::delim c) {
}

void parser::token_symbol(location loc, std::string text) {
	struct opdesc {
		ast::binary::opcode id;
		precedence prec;
	};
	static std::map<std::string, opdesc> ops = {
		{";", {ast::branch::sequence, precedence::structure}},
		{",", {ast::branch::tuple, precedence::structure}},
		{":", {ast::branch::declare, precedence::binding}},
		{":=", {ast::branch::define, precedence::binding}},
		{"::=", {ast::branch::typealias, precedence::binding}},
		{"<-", {ast::branch::assign, precedence::binding}},
		{"->", {ast::branch::capture, precedence::binding}},
		{"=", {ast::branch::eq, precedence::relation}},
		{"<", {ast::branch::lt, precedence::relation}},
		{">", {ast::branhc::gt, precedence::relation}},
		{"!=", {ast::branch::neq, precedence::relation}},
		{"!<", {ast::branch::nlt, precedence::relation}},
		{"!>", {ast::branch::ngt, precedence::relation}},
		{"+", {ast::branch::add, precedence::additive}},
		{"-", {ast::branch::sub, precedence::additive}},
		{"|", {ast::branch::disjoin, precedence::additive}},
		{"^", {ast::branch::exclude, precedence::additive}},
		{"..", {ast::branch::range, precedence::additive}},
		{"!", {ast::branch::exclude, precedence::additive}},
		{"*", {ast::branch::mul, precedence::multiplicative}},
		{"/", {ast::branch::div, precedence::multiplicative}},
		{"%", {ast::branch::rem, precedence::multiplicative}},
		{"&", {ast::branch::conjoin, precedence::multiplicative}},
		{"<<", {ast::branch::shl, precedence::multiplicative}},
		{">>", {ast::branch::shr, precedence::multiplicative}},
		{".", {ast::branch::pipeline, precedence::primary}}
	};
	auto iter = ops.find(text);
	if (iter == ops.end()) {
		err.parser_unexpected(l);
		return;
	}
	precedence prec = iter->second.prec;
	if (expecting_term) {
		// Allow this operator to be used as a prefix by giving it a blank
		// left operand. The semantic analyzer can figure out later whether
		// that actually makes any sense.
		out.build_blank(loc);
		prec = precedence::prefix;
	}
	push(iter->second.id, prec, l);
}

void parser::commit() {
	out.build_branch(ops.top().id, ops.top().loc);
	ops.pop();
}

void parser::reduce(precedence prec) {
	switch (prec) {
		case precedence::binding:
		case precedence::prefix:
			// right-associative
			while (!ops.empty() && prec < ops.top().prec) {
				commit();
			}
			break;
		default:
			// left-associative
			while (!ops.empty() && prec <= ops.top().prec) {
				commit();
			}
			break;
	}
}

void parser::term(ast::leaf::tag id, std::string text, location loc) {
	if (!expecting_term) {
		push(ast::branch::apply, precedence::primary, loc);
	}
	out.build_leaf(id, text, loc);
	expecting_term = false;
}

void parser::push(ast::branch::tag id, precedence prec, location loc) {
	reduce(prec);
	ops.push({id, prec, loc});
	expecting_term = true;
}

void parser::state::close(location l, ast::ostream &out, error &err) {
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
	while (!ops.empty()) commit();
	assert(vals.empty());
	if (exp) out << cur();
}

bool parser::state::expecting_term() {
	return !exp;
}


void parser::state::term(ast::node *n) {
	if (!expecting_term()) {
		prep(precedence::primary);
		ops.push({ast::binary::apply, precedence::primary, n->loc()});
		vals.push(std::move(exp));
	}
	emit(n);
}

void parser::state::symbol(location l, std::string text, error &err) {
	enum {
		mode_infix = 0,
		mode_prefix,
		mode_either,
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
		{"-", {ast::binary::sub, precedence::additive, mode_either}},
		{"|", {ast::binary::disjoin, precedence::additive}},
		{"^", {ast::binary::exclude, precedence::additive}},
		{"..", {ast::binary::range, precedence::additive}},
		{"!", {ast::binary::exclude, precedence::additive, mode_prefix}},
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
		case mode_infix:
			infix({op.id, op.prec, l}, err);
			break;
		case mode_prefix:
			prefix({op.id, precedence::prefix, l}, err);
			break;
		case mode_either:
			if (!expecting_term()) {
				infix({op.id, op.prec, l}, err);
			} else {
				prefix({op.id, precedence::prefix, l}, err);
			}
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
	context.close(l, out, err);
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
	struct tanker: public ast::ostream {
		virtual ostream &operator<<(ast::ptr &&n) override {
			exp = std::move(n);
			return *this;
		}
		ast::ptr exp;
	} subscope;
	context.close(r, subscope, err);
	// don't use cur() to build these subexpressions, because it requires that
	// there actually be a current expression, which is true for all infix and
	// prefix operators. A pair of group delimiters might not contain anything,
	// and that's a legal expression.
	ast::group *exp;
	switch (top.type) {
		case token::delim::paren:
			if (g != top.type) err.parser_expected(r, "')'", top.loc);
			exp = new ast::parens(top.loc, std::move(subscope.exp), r);
			break;
		case token::delim::bracket:
			if (g != top.type) err.parser_expected(r, "']'", top.loc);
			exp = new ast::brackets(top.loc, std::move(subscope.exp), r);
			break;
		case token::delim::brace:
			if (g != top.type) err.parser_expected(r, "'}'", top.loc);
			exp = new ast::braces(top.loc, std::move(subscope.exp), r);
			break;
	}
	context = std::move(top.context);
	context.term(exp);
}

void parser::token_symbol(location l, std::string text) {
	context.symbol(l, text, err);
}

