// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "parser.h"
#include <map>

void parser::parse(token::type type, std::string text, location loc) {
	switch (type) {
		case token::eof: parse_eof(text, loc); break;
		case token::number: parse_number(text, loc); break;
		case token::identifier: parse_identifier(text, loc); break;
		case token::string: parse_string(text, loc); break;
		case token::symbol: parse_symbol(text, loc); break;
		case token::delimiter: parse_delimiter(text, loc); break;
	}
}

void parser::parse_eof(std::string text, location loc) {
	if (outer.empty()) {
		close(loc);
		out.emit_eof(loc);
	} else {
		err.report(outer.top().loc, "opening delimiter is never closed");
	}
}

void parser::parse_number(std::string text, location loc) {
	prep_term(loc);
	out.emit_number(text, loc);
}

void parser::parse_identifier(std::string text, location loc) {
	prep_term(loc);
	out.emit_identifier(text, loc);
}

void parser::parse_string(std::string text, location loc) {
	prep_term(loc);
	out.emit_string(text, loc);
}

void parser::parse_symbol(std::string text, location loc) {
	struct opdesc {
		syntax::branch id;
		precedence prec;
	};
	static std::map<std::string, opdesc> ops = {
		{":", {syntax::declare, precedence::binding}},
		{":=", {syntax::define, precedence::binding}},
		{"::=", {syntax::typealias, precedence::binding}},
		{"<-", {syntax::assign, precedence::binding}},
		{"->", {syntax::capture, precedence::binding}},
		{"=", {syntax::eq, precedence::relation}},
		{"<", {syntax::lt, precedence::relation}},
		{">", {syntax::gt, precedence::relation}},
		{"!=", {syntax::neq, precedence::relation}},
		{"!<", {syntax::nlt, precedence::relation}},
		{"!>", {syntax::ngt, precedence::relation}},
		{"+", {syntax::add, precedence::additive}},
		{"-", {syntax::sub, precedence::additive}},
		{"&", {syntax::and_join, precedence::multiplicative}},
		{"|", {syntax::or_join, precedence::additive}},
		{"^", {syntax::xor_join, precedence::additive}},
		{"!&", {syntax::nand_join, precedence::multiplicative}},
		{"!|", {syntax::nor_join, precedence::additive}},
		{"!^", {syntax::xnor_join, precedence::additive}},
		{"..", {syntax::range, precedence::additive}},
		{"!", {syntax::nand_join, precedence::additive}},
		{"*", {syntax::mul, precedence::multiplicative}},
		{"/", {syntax::div, precedence::multiplicative}},
		{"%", {syntax::rem, precedence::multiplicative}},
		{"<<", {syntax::shl, precedence::multiplicative}},
		{">>", {syntax::shr, precedence::multiplicative}},
		{".", {syntax::pipe, precedence::primary}}
	};
	auto iter = ops.find(text);
	if (iter == ops.end()) {
		err.report(loc, "syntax error: unknown operator");
		return;
	}
	precedence prec = prep_operator(loc, iter->second.prec);
	push({loc, iter->second.id, prec, text});
}

void parser::parse_delimiter(std::string text, location loc) {
	switch (text.size() == 1? text.front(): 0) {
		case '(': case '[': case '{': {
			static std::map<std::string, std::string> delims = {
				{"(", ")"},
				{"[", "]"},
				{"{", "}"},
			};
			auto iter = delims.find(text);
			if (iter == delims.end()) {
				err.report(loc, "unknown delimiter");
				return;
			}
			if (!expecting_term) {
				push({loc, syntax::apply, precedence::primary});
			}
			context current{loc, iter->second, std::move(ops)};
			outer.push(std::move(current));
			expecting_term = true;
		} break;
		case ')': case ']': case '}': {
			if (outer.empty()) {
				err.report(loc, "unexpected closing delimiter");
				return;
			}
			if (outer.top().closer != text) {
				err.report(loc, "mismatched closing delimiter");
				return;
			}
			close(loc);
			ops = std::move(outer.top().ops);
			outer.pop();
			expecting_term = false;
		} break;
		case ';': {
			precedence prec = prep_operator(loc, precedence::sequence);
			push({loc, syntax::sequence, prec, text});
		} break;
		case ',': {
			precedence prec = prep_operator(loc, precedence::binding);
			push({loc, syntax::pair, prec, text});
		} break;
		default: {
			err.report(loc, "syntax error: unknown delimiter");
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
		out.emit_branch(op.id, op.text, op.loc);
		ops.pop();
	}
}

void parser::prep_term(location loc) {
	if (!expecting_term) {
		push({loc, syntax::apply, precedence::primary});
	}
	expecting_term = false;
}

parser::precedence parser::prep_operator(location loc, precedence prec) {
	if (expecting_term) {
		out.emit_null(loc);
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
		out.emit_null(loc);
	}
	reduce(precedence::none);
}
