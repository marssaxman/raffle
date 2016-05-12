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
		out.emit(ast::eof, text, loc);
	} else {
		err.report(outer.top().loc, "opening delimiter is never closed");
	}
}

void parser::parse_number(std::string text, location loc) {
	prep_term(loc);
	out.emit(ast::number, text, loc);
}

void parser::parse_identifier(std::string text, location loc) {
	prep_term(loc);
	out.emit(ast::identifier, text, loc);
}

void parser::parse_string(std::string text, location loc) {
	prep_term(loc);
	out.emit(ast::string, text, loc);
}

void parser::parse_symbol(std::string text, location loc) {
	struct opdesc {
		ast::type id;
		precedence prec;
	};
	static std::map<std::string, opdesc> ops = {
		{":", {ast::declare, precedence::binding}},
		{":=", {ast::define, precedence::binding}},
		{"::=", {ast::typealias, precedence::binding}},
		{"<-", {ast::assign, precedence::binding}},
		{"->", {ast::capture, precedence::binding}},
		{"=", {ast::eq, precedence::relation}},
		{"<", {ast::lt, precedence::relation}},
		{">", {ast::gt, precedence::relation}},
		{"!=", {ast::neq, precedence::relation}},
		{"!<", {ast::nlt, precedence::relation}},
		{"!>", {ast::ngt, precedence::relation}},
		{"+", {ast::add, precedence::additive}},
		{"-", {ast::sub, precedence::additive}},
		{"&", {ast::and_join, precedence::multiplicative}},
		{"|", {ast::or_join, precedence::additive}},
		{"^", {ast::xor_join, precedence::additive}},
		{"!&", {ast::nand_join, precedence::multiplicative}},
		{"!|", {ast::nor_join, precedence::additive}},
		{"!^", {ast::xnor_join, precedence::additive}},
		{"..", {ast::range, precedence::additive}},
		{"!", {ast::nand_join, precedence::additive}},
		{"*", {ast::mul, precedence::multiplicative}},
		{"/", {ast::div, precedence::multiplicative}},
		{"%", {ast::rem, precedence::multiplicative}},
		{"<<", {ast::shl, precedence::multiplicative}},
		{">>", {ast::shr, precedence::multiplicative}},
		{".", {ast::pipe, precedence::primary}}
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
				push({loc, ast::apply, precedence::primary});
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
			push({loc, ast::sequence, prec, text});
		} break;
		case ',': {
			precedence prec = prep_operator(loc, precedence::binding);
			push({loc, ast::pair, prec, text});
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
		out.emit(op.id, op.text, op.loc);
		ops.pop();
	}
}

void parser::prep_term(location loc) {
	if (!expecting_term) {
		push({loc, ast::apply, precedence::primary});
	}
	expecting_term = false;
}

parser::precedence parser::prep_operator(location loc, precedence prec) {
	if (expecting_term) {
		out.emit(ast::null, std::string(), loc);
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
		out.emit(ast::null, std::string(), loc);
	}
	reduce(precedence::none);
}
