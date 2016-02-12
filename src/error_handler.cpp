// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include "error_handler.h"

void error_handler::report(lexer::position p, std::string message)
{
	std::cerr << p.row << ":" << p.col << ": " << message << std::endl;
}

void error_handler::token_unknown(lexer::position p, char c)
{
	report(p, "unknown character '" + std::string(1, c) + "'");
}

void error_handler::token_nonterminated(lexer::position p)
{
	report(p, "nonterminated string");
}

void error_handler::parse_unexpected(lexer::position p)
{
	report(p, "syntax error");
}

void error_handler::parse_mismatched_paren(lexer::position p)
{
	report(p, "no match for this parenthesis");
}

