// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"
#include <map>
#include <assert.h>

void parser::token_eof(location loc) {
	if (!outer.empty()) {
		err.parser_unclosed_delimiter(outer.top().loc);
		return;
	}
	close(loc);
}

void parser::token_number(location loc, std::string text) {
	prep_term(loc);
	out.build_leaf(ast::leaf::number, text, loc);
}

void parser::token_identifier(location loc, std::string text) {
	prep_term(loc);
	out.build_leaf(ast::leaf::symbol, text, loc);
}

void parser::token_string(location loc, std::string text) {
	prep_term(loc);
	out.build_leaf(ast::leaf::string, text, loc);
}

void parser::token_underscore(location loc) {
	prep_term(loc);
	out.build_blank(loc);
}

void parser::token_open(location loc, token::delim c) {
	prep_term(loc);
	context current{c, loc, std::move(ops)};
	outer.push(std::move(current));
	expecting_term = true;
}

void parser::token_close(location loc, token::delim c) {
	if (outer.empty()) {
		err.parser_unexpected_delimiter(loc);
		return;
	}
	if (outer.top().type != c) {
		err.parser_mismatched_delimiter(loc);
	}
	close(loc);
	ops = std::move(outer.top().ops);
	outer.pop();
	expecting_term = false;
}

void parser::token_symbol(location loc, std::string text) {
	struct opdesc {
		ast::branch::tag id;
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
		{">", {ast::branch::gt, precedence::relation}},
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
		err.parser_unexpected(loc);
		return;
	}
	precedence prec = iter->second.prec;
	if (expecting_term) {
		out.build_blank(loc);
		prec = precedence::prefix;
	}
	push(iter->second.id, prec, loc);
}

void parser::reduce(precedence prec) {
	bool rightassoc = false;
	switch (prec) {
		case precedence::binding:
		case precedence::prefix: rightassoc = true;
	}
	while (!ops.empty()) {
		if (prec > ops.top().prec) break;
		if (rightassoc && prec == ops.top().prec) break;
		out.build_branch(ops.top().id, ops.top().loc);
		ops.pop();
	}
}

void parser::prep_term(location loc) {
	if (!expecting_term) {
		push(ast::branch::apply, precedence::primary, loc);
	}
	expecting_term = false;
}

void parser::push(ast::branch::tag id, precedence prec, location loc) {
	reduce(prec);
	ops.push({id, prec, loc});
	expecting_term = true;
}

void parser::close(location loc) {
	if (expecting_term) {
		out.build_blank(loc);
	}
	reduce(precedence::none);
}
