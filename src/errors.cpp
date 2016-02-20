// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "errors.h"

void errors::print_loc(location l) {
	position p = l.begin;
	std::cerr << p.row << ":" << p.col;
}

void errors::report(location l, std::string message) {
	print_loc(l);
	std::cerr << ": " << message << std::endl;
}

void errors::report(location l, std::string message, location prev) {
	print_loc(l);
	std::cerr << ": " << message << " (see ";
	print_loc(prev);
	std::cerr << ")" << std::endl;
}

void errors::lexer_unknown(location l, char c) {
	report(l, "unknown character '" + std::string(1, c) + "'");
}

void errors::lexer_nonterminated(location l) {
	report(l, "nonterminated string");
}

void errors::parser_unexpected(location l) {
	report(l, "syntax error");
}

void errors::parser_missing_left_operand(location l) {
	report(l, "expected expression before this operator");
}

void errors::parser_missing_right_operand(location l) {
	report(l, "expected expression after this operator");
}

void errors::parser_expected(location l, std::string what, location prev) {
	report(l, "expected " + what, prev);
}

void errors::parser_semicolon_in_tuple(location l) {
	report(l, "unexpected sequence delimiter in structure context");
}

void errors::parser_comma_outside_tuple(location l) {
	report(l, "unexpected structure delimiter in sequence context");
}

void errors::resolver_undefined(location l) {
	report(l, "undefined symbol");
}

void errors::resolver_redefined(location cur, location prev) {
	report(cur, "symbol already defined", prev);
}

void errors::resolver_unexpected_definition(location l) {
	report(l, "unexpected definition expression");
}

void errors::resolver_unexpected_target(location l) {
	report(l, "unexpected binding target expression");
}

void errors::resolver_unexpected_constraint(location l) {
	report(l, "unexpected type constraint expression");
}

void errors::resolver_unexpected_wildcard(location l) {
	report(l, "unexpected wildcard outside pattern expression");
}
