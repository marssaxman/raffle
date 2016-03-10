// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"
#include <map>

void parser::token_eof(location loc) {
	if (!outer.empty()) {
		err.report(outer.top().loc, "opening delimiter is never closed");
		return;
	}
	close(loc);
}

void parser::token_number(std::string text, location loc) {
	prep_term(loc);
	out.ast_leaf(loc, ast::leaf::number, text);
}

void parser::token_identifier(std::string text, location loc) {
	prep_term(loc);
	out.ast_leaf(loc, ast::leaf::identifier, text);
}

void parser::token_string(std::string text, location loc) {
	prep_term(loc);
	out.ast_leaf(loc, ast::leaf::string, text);
}

void parser::token_underscore(location loc) {
	prep_term(loc);
	out.ast_atom(loc, ast::atom::wildcard);
}

void parser::token_open(std::string text, location loc) {
	static std::map<std::string, std::string> delims = {
		{"(", ")"},
		{"[", "]"},
		{"{", "}"},
	};
	auto iter = delims.find(text);
	if (iter == delims.end()) {
		err.report(loc, "syntax error: unknown opening delimiter");
		return;
	}
	if (!expecting_term) {
		push({loc, ast::branch::apply, precedence::primary});
	}
	context current{loc, iter->second, std::move(ops)};
	outer.push(std::move(current));
	expecting_term = true;
}

void parser::token_close(std::string text, location loc) {
	if (outer.empty()) {
		err.report(loc, "no opening to match this closing delimiter");
		return;
	}
	if (outer.top().closer != text) {
		err.report(loc, "wrong closing delimiter for this expression");
		return;
	}
	close(loc);
	ops = std::move(outer.top().ops);
	outer.pop();
	expecting_term = false;
}

void parser::token_symbol(std::string text, location loc) {
	struct opdesc {
		ast::branch id;
		precedence prec;
	};
	static std::map<std::string, opdesc> ops = {
		{";", {ast::branch::sequence, precedence::sequence}},
		{",", {ast::branch::pair, precedence::binding}},
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
		{"&", {ast::branch::and_join, precedence::multiplicative}},
		{"|", {ast::branch::or_join, precedence::additive}},
		{"^", {ast::branch::xor_join, precedence::additive}},
		{"!&", {ast::branch::nand_join, precedence::multiplicative}},
		{"!|", {ast::branch::nor_join, precedence::additive}},
		{"!^", {ast::branch::xnor_join, precedence::additive}},
		{"..", {ast::branch::range, precedence::additive}},
		{"!", {ast::branch::nand_join, precedence::additive}},
		{"*", {ast::branch::mul, precedence::multiplicative}},
		{"/", {ast::branch::div, precedence::multiplicative}},
		{"%", {ast::branch::rem, precedence::multiplicative}},
		{"<<", {ast::branch::shl, precedence::multiplicative}},
		{">>", {ast::branch::shr, precedence::multiplicative}},
		{".", {ast::branch::pipe, precedence::primary}}
	};
	auto iter = ops.find(text);
	if (iter == ops.end()) {
		err.report(loc, "syntax error: unknown operator");
		return;
	}
	precedence prec = iter->second.prec;
	if (expecting_term) {
		out.ast_atom(loc, ast::atom::null);
		prec = precedence::prefix;
	}
	push({loc, iter->second.id, prec, text});
}

void parser::reduce(precedence prec) {
	bool rightassoc = false;
	switch (prec) {
		case precedence::binding:
		case precedence::prefix: rightassoc = true;
		default: rightassoc = false;
	}
	while (!ops.empty()) {
		if (prec > ops.top().prec) break;
		if (rightassoc && prec == ops.top().prec) break;
		oprec &op = ops.top();
		out.ast_branch(op.loc, op.id, op.text);
		ops.pop();
	}
}

void parser::prep_term(location loc) {
	if (!expecting_term) {
		push({loc, ast::branch::apply, precedence::primary});
	}
	expecting_term = false;
}

void parser::push(oprec op) {
	reduce(op.prec);
	ops.push(op);
	expecting_term = true;
}

void parser::close(location loc) {
	if (expecting_term) {
		out.ast_atom(loc, ast::atom::null);
	}
	reduce(precedence::none);
}
