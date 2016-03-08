// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "errors.h"

void errors::print_loc(location l) {
	position p = l.begin;
	std::cerr << p.row() << ":" << p.col();
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

void errors::parser_mismatched_delimiter(location l) {
	report(l, "wrong closing delimiter for this expression");
}

void errors::parser_unclosed_delimiter(location l) {
	report(l, "opening delimiter is never closed");
}

void errors::parser_unexpected_delimiter(location l) {
	report(l, "no opening to match this closing delimiter");
}

void errors::binder_unimplemented(location l) {
	report(l, "mars hasn't gotten around to building that yet");
}

