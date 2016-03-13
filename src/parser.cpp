// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"
#include <map>

void parser::operator()(token::eof t) {
	if (!outer.empty()) {
		err.report(outer.top().loc, "opening delimiter is never closed");
		return;
	}
	close(t.loc);
}

void parser::operator()(token::number t) {
	prep_term(t.loc);
	out.ast_leaf(t.loc, ast::leaf::number, t.text);
}

void parser::operator()(token::identifier t) {
	prep_term(t.loc);
	out.ast_leaf(t.loc, ast::leaf::identifier, t.text);
}

void parser::operator()(token::string t) {
	prep_term(t.loc);
	out.ast_leaf(t.loc, ast::leaf::string, t.text);
}

void parser::operator()(token::symbol t) {
	struct opdesc {
		ast::branch id;
		precedence prec;
	};
	static std::map<std::string, opdesc> ops = {
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
	auto iter = ops.find(t.text);
	if (iter == ops.end()) {
		err.report(t.loc, "syntax error: unknown operator");
		return;
	}
	precedence prec = prep_operator(t.loc, iter->second.prec);
	push({t.loc, iter->second.id, prec, t.text});
}

void parser::operator()(token::delimiter t) {
	switch (t.text.size() == 1? t.text.front(): 0) {
		case '(': case '[': case '{': {
			static std::map<std::string, std::string> delims = {
				{"(", ")"},
				{"[", "]"},
				{"{", "}"},
			};
			auto iter = delims.find(t.text);
			if (iter == delims.end()) {
				err.report(t.loc, "unknown delimiter");
				return;
			}
			if (!expecting_term) {
				push({t.loc, ast::branch::apply, precedence::primary});
			}
			context current{t.loc, iter->second, std::move(ops)};
			outer.push(std::move(current));
			expecting_term = true;
		} break;
		case ')': case ']': case '}': {
			if (outer.empty()) {
				err.report(t.loc, "unexpected closing delimiter");
				return;
			}
			if (outer.top().closer != t.text) {
				err.report(t.loc, "mismatched closing delimiter");
				return;
			}
			close(t.loc);
			ops = std::move(outer.top().ops);
			outer.pop();
			expecting_term = false;
		} break;
		case ';': {
			precedence prec = prep_operator(t.loc, precedence::sequence);
			push({t.loc, ast::branch::sequence, prec, t.text});
		} break;
		case ',': {
			precedence prec = prep_operator(t.loc, precedence::binding);
			push({t.loc, ast::branch::pair, prec, t.text});
		} break;
		default: {
			err.report(t.loc, "syntax error: unknown delimiter");
		}
	}
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

parser::precedence parser::prep_operator(location loc, precedence prec) {
	if (expecting_term) {
		out.ast_atom(loc, ast::atom::null);
		prec = precedence::prefix;
	}
	expecting_term = true;
	return prec;
}

void parser::push(oprec op) {
	reduce(op.prec);
	ops.push(op);
}

void parser::close(location loc) {
	if (expecting_term) {
		out.ast_atom(loc, ast::atom::null);
	}
	reduce(precedence::none);
}
