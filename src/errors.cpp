// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "errors.h"

void errors::report(location l, std::string message) {
	position p = l.begin;
	std::cerr << p.row << ":" << p.col << ": " << message << std::endl;
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

void errors::parser_mismatched_group(location l) {
	report(l, "unmatched grouping delimiter");
}

void errors::parser_mismatched_separator(location l) {
	report(l, "unexpected separator");
}

void errors::resolver_undefined(location l) {
	report(l, "undefined symbol");
}

void errors::resolver_redefined(location cur, location prev) {
	position p = prev.begin;
	std::string def = std::to_string(p.row) + ":" + std::to_string(p.col);
	report(cur, "symbol already defined at " + def);
}

